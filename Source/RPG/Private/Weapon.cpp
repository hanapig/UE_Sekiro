// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "SAICharacter.h"
#include "SCharacter.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	
	RootComponent = WeaponMesh;

	UBoxComponent* Box = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));

	if (Box)
	{
		Box->SetHiddenInGame(false);

		Box->SetVisibility(false, true);

		Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponCollision);
	}

	if (GetOwner())
	{
		Box->IgnoreActorWhenMoving(GetOwner(), true);
	}
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	UBoxComponent* Box = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	if (Box)
	{
		Box->OnComponentBeginOverlap.Clear();  
		Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponCollision);
		
		Box->SetCollisionObjectType(ECC_GameTraceChannel1);
		Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Box->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetVisibility(true);

		if (GetOwner())
		{
			Box->IgnoreActorWhenMoving(GetOwner(), true);
			
		}
		
	}
	
}


void ::AWeapon::StartBox()
{
	UBoxComponent* Box = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	
	if (Box)
	{
		
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetVisibility(true);
	}

}

void ::AWeapon::StopBox()
{
	UBoxComponent* Box = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	
	if (Box)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Box->SetVisibility(false);
	}
}


void AWeapon::OnWeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	if (OtherComp->GetCollisionObjectType() == ECC_Pawn)
	{
		ASCharacter* Player = Cast<ASCharacter>(OtherActor);
		if (Player)
		{
			return;
		}
	}

	ASAICharacter* Enemy = Cast<ASAICharacter>(OtherActor);

	UAnimInstance* EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
	if (Enemy)
	{
		if (OtherComp->ComponentHasTag(FName("PBox")))
		{
			ASCharacter* OwnerCharacter = Cast<ASCharacter>(GetOwner());
			
			if (OwnerCharacter->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Stiff"))) || OwnerCharacter->BlockTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Hurt")))||Enemy->AttackTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Combo3"))))
			{
		
				Enemy->OnInjured();
				Enemy->AttackTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Combo3")));
				return; 
			}

			
			if (OwnerCharacter)
			{
				int32 AttackStage = OwnerCharacter->GetCurrentAttackStage();
				switch (AttackStage)
				{
				case 1:
					EnemyAnimInstance->Montage_Play(Enemy->Blocked_Combo5);
					OwnerCharacter->OnBlocked();
					break;

				case 2:
					Enemy->OnParried_L();
					OwnerCharacter->OnBlocked_L();
					break;

				default:
					break;
				}
			}
		}
	}
}




