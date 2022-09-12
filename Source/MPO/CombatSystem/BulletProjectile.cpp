#include "BulletProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/AssetManager.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetCollisionProfileName("OverlapOnlyPawn");
	Collision->CanCharacterStepUpOn = ECB_No;
	Collision->AreaClass = nullptr;
	Collision->SetShouldUpdatePhysicsVolume(false);
	Collision->SetCanEverAffectNavigation(false);
	Collision->SetGenerateOverlapEvents(true);

	RootComponent = Collision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bBounceAngleAffectsFriction = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.2;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 50;
	ProjectileMovement->MinFrictionFraction = 0.5;
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ABulletProjectile::OnBounce);
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABulletProjectile::OnStop);

	DamageType = DamageType ? DamageType : UDamageType::StaticClass();

	this->SetCanBeDamaged(false);
}

void ABulletProjectile::OnStop(const FHitResult& ImpactResult) {
	Destroy();
}

void ABulletProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity) {
	auto AffectedActor = ImpactResult.GetActor();
	if (AffectedActor == GetInstigator()) {
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetEmitterFromVelocity(ImpactVelocity.Size()), FTransform(ImpactResult.Location));
	auto AttackAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ImpactVelocity.GetSafeNormal(), ImpactResult.ImpactNormal))) - 90;

	Bounces++;
	if (AttackAngle > MaxBounceAngle || Bounces > MaxBounces) {
		Destroy();
	}
}

void ABulletProjectile::NotifyActorBeginOverlap(AActor* OtherActor) {
	if (auto Pawn = Cast<APawn>(OtherActor)) {
		UGameplayStatics::ApplyDamage(
			Pawn,
			GetVelocity().Size() / InitialSpeedMultiplier * Mass * 1000,
			this->GetInstigator()->GetController(),
			this->GetInstigator(),
			DamageType);

		if (!bCanPenetrate) {
			Destroy();
		}
	}
}

UParticleSystem* ABulletProjectile::GetEmitterFromVelocity(float Velocity) {
	auto Index = UKismetMathLibrary::MapRangeClamped(Velocity, 0, 30000, 0, HitEmitters.Num() - 1);
	return HitEmitters[Index];
}

void ABulletProjectile::Init(UAmmoItemData* AmmoData, FVector Direction) {
	if (!AmmoData) {
		return;
	}

	// projectile mass from g to kg
	Mass = AmmoData->Mass / 1000;
	auto Radius = AmmoData->Diameter / 2;

	bCanPenetrate = AmmoData->bCanPenetrate;

	// extent size with conversion from mm to cm
	Collision->SetBoxExtent(FVector(AmmoData->Length, Radius, Radius) / 10, true);

	ProjectileMovement->Velocity = Direction.GetSafeNormal();
	ProjectileMovement->InitialSpeed = AmmoData->Speed * InitialSpeedMultiplier; // m/s to cm/s
	Tracer = AmmoData->Tracer;

	if (!Tracer.IsValid()) {
		auto& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(
			Tracer.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ABulletProjectile::OnTracerLoaded));
	}
	else {
		OnTracerLoaded();
	}
}

void ABulletProjectile::OnTracerLoaded() {
	UGameplayStatics::SpawnEmitterAttached(Tracer.Get(), Collision);
}