// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TempestTenacityComponent.h"
#include "Objects/TempestBaseAttributeObject.h"

// Sets default values for this component's properties
UTempestTenacityComponent::UTempestTenacityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UTempestTenacityComponent::CompareTenacityDifference_Implementation(AActor* Other, AActor*& OutSmallerActor)
{
	AActor* Owner = GetOwner();
	if (!Owner || !Other)return 0.0f;
	UTempestAttributesComponents* OwnerAttrComp = Owner->FindComponentByClass<UTempestAttributesComponents>();
	UTempestAttributesComponents* OtherAttrComp = Other->FindComponentByClass<UTempestAttributesComponents>();
	if (!OwnerAttrComp || !OtherAttrComp)return 0.0f;
	UTempestBaseAttributeObject* OwnerTenacityAttrObj = OwnerAttrComp->GetAttributeOfGameplayTag((FGameplayTag::RequestGameplayTag("Attribute.CombatState.Resilience")));
	UTempestBaseAttributeObject* OtherTenacityAttrObj = OtherAttrComp->GetAttributeOfGameplayTag((FGameplayTag::RequestGameplayTag("Attribute.CombatState.Resilience")));
	if (!OwnerTenacityAttrObj || !OtherTenacityAttrObj) return 0.0f;
	
	float OwnerCurrentAttr =  OwnerTenacityAttrObj->AttributeValues.AttributeValue;
	float OtherCurrentAttr =  OtherTenacityAttrObj->AttributeValues.AttributeValue;
	TenacityCompareDifference = abs(OwnerCurrentAttr - OtherCurrentAttr);
	if (FMath::Abs(OwnerCurrentAttr - OtherCurrentAttr) <= KINDA_SMALL_NUMBER)
		OutSmallerActor = nullptr; 
	else
		OutSmallerActor = (OwnerCurrentAttr < OtherCurrentAttr) ? Owner : Other;

	OnTenacityDifferenceChanged.Broadcast(TenacityCompareDifference);
	return TenacityCompareDifference;
}

float UTempestTenacityComponent::GetCurrentTenacityValue() const
{
	AActor* Owner = GetOwner();
	if (!Owner)return 0.0f;
	UTempestAttributesComponents* AttrComp = Owner->FindComponentByClass<UTempestAttributesComponents>();
	if (!AttrComp)return 0.0f;
	UTempestBaseAttributeObject* TenacityAttr = AttrComp->GetAttributeOfGameplayTag(FGameplayTag::RequestGameplayTag("Attribute.CombatState.Resilience"));
	if (!TenacityAttr)return 0.0f;
	return TenacityAttr->AttributeValues.AttributeValue;
}

void UTempestTenacityComponent::SetCurrentTenacityValue(float NewValue)
{
	AActor* Owner = GetOwner();
	if (!Owner)return ;
	UTempestAttributesComponents* AttrComp = Owner->FindComponentByClass<UTempestAttributesComponents>();
	if (!AttrComp)return ;
	UTempestBaseAttributeObject* TenacityAttr = AttrComp->GetAttributeOfGameplayTag(FGameplayTag::RequestGameplayTag("Attribute.CombatState.Resilience"));
	if (!TenacityAttr)return ;
	TenacityAttr->AttributeValues.AttributeValue = NewValue;
}

void UTempestTenacityComponent::ModifyCurrentTenacityValue(float ModifyBy)
{
	AActor* Owner = GetOwner();
	if (!Owner)return ;
	UTempestAttributesComponents* AttrComp = Owner->FindComponentByClass<UTempestAttributesComponents>();
	if (!AttrComp)return ;
	UTempestBaseAttributeObject* TenacityAttr = AttrComp->GetAttributeOfGameplayTag(FGameplayTag::RequestGameplayTag("Attribute.CombatState.Resilience"));
	if (!TenacityAttr)return ;
	TenacityAttr->ModifyAttribute(ModifyBy);
}


