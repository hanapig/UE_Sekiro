// Copyright 2022, Developed by Aamn Chahrour & Samrudh Sunil, Published by Inherited Tempest. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/TempestBaseComponent.h"
#include "GameplayTagContainer.h"
#include "Objects/TempestBaseAttributeModifier.h"
#include "TempestAttributesComponents.generated.h"

class UTempestBaseAttributeObject;
class UTempestBaseAttributeModifier;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class TEMPESTCOMBATFRAMEWORK_API UTempestAttributesComponents : public UTempestBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTempestAttributesComponents();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute Base Variables")
	TArray<UTempestBaseAttributeObject*> CreatedAttributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute Base Variables")
	TArray<UTempestBaseAttributeModifier*> CreatedAttributeModifiers;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnOwnerDestroyed(AActor* DestroyedActor) override;

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void AssignAttributes(TArray<UTempestBaseAttributeObject*> AttributesToAssign);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ClearAttributes();

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ClearAttributeModifiers();

	/* Construction */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ConstructAttributeOfClass(TSubclassOf<UTempestBaseAttributeObject> AttributeToConstruct, UTempestBaseAttributeObject*& ConstructedAttribute);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ConstructAttributeModifierOfClass(TSubclassOf<UTempestBaseAttributeModifier> AttributeModifierToConstruct, UTempestBaseAttributeModifier*& ConstructedAttributeModifier);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetAttributeOfClass(TSubclassOf<UTempestBaseAttributeObject> AttributeToSearch, UTempestBaseAttributeObject*& FoundAttribute);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetAttributeModifierOfClass(TSubclassOf<UTempestBaseAttributeModifier> AttributeModifierToSearch, UTempestBaseAttributeModifier*& FoundAttributeModifier);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	void GetAttributeModifierOfGamplayTag(FGameplayTag AttributeModifierTagToSearch, UTempestBaseAttributeModifier*& FoundAttributeModifier);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	UTempestBaseAttributeObject*& GetAttributeOfGameplayTag(FGameplayTag AttributeGameplayTag);


	UFUNCTION(BlueprintCallable, Category = "Construction")
	void RemoveFromAttributeModifiers(UTempestBaseAttributeModifier* AttributeModifierToRemove);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void AddToAttributeModifiers(UTempestBaseAttributeModifier* AttributeModifierToAdd);


	
};
