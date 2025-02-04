// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSItemPad.h"
#include "NiagaraComponent.h"
#include "AdvancedShooting/Interface/CharacterInterface.h"
#include "AdvancedShooting/Struct/ItemPad.h"
#include "Components/CapsuleComponent.h"




// Sets default values
AJMSItemPad::AJMSItemPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ItemPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad"));
	if (ItemPad)
	{
		ItemPad->SetupAttachment(Root);
		static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/UI/BP/DT_ItemPad.DT_ItemPad"));
		if (DataTableObj.Succeeded())
		{
			ItemPadDataTable = DataTableObj.Object;
			ItemHandle.DataTable = ItemPadDataTable;

		}
		



	
	}

	ItemPadWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	ItemPadWeapon->SetupAttachment(Root);

	ItemPadNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	ItemPadNiagaraEffect->SetupAttachment(Root);

	ItemPadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	ItemPadCapsule->SetupAttachment(Root);
	ItemPadCapsule->SetCapsuleSize(100.f, 100.f);
	ItemPadCapsule->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::AJMSItemPad::OnComponentBeginOverlap);
}

#if WITH_EDITOR


void AJMSItemPad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (ItemPadDataTable != nullptr && ItemPadNiagaraEffect != nullptr)
	{
		FItemPad* ItemPadStruct = ItemPadDataTable->FindRow<FItemPad>(ItemHandle.RowName,TEXT("GetRow"),true);
		if (ItemPadStruct)
		{
			UStaticMesh* PadMesh = nullptr;
			FVector MeshColor = FVector::ZeroVector;
			switch (ItemPadStruct->ItemType)
			{
			case EItemPadType::Health:
				PadMesh = HealthPad.Get();
				MeshColor = FVector(0,0,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),HealthLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(nullptr);
				break;
			case EItemPadType::Shield:
				PadMesh = ShieldPad.Get();
				MeshColor = FVector(0,1,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),ShieldLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(nullptr);
				break;
			case EItemPadType::PistolClip:
				PadMesh = PistolPad.Get();
				MeshColor = FVector(1,0,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),PistolLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(PistolMesh);
				break;
			case EItemPadType::RifleClip:
				PadMesh = RiflePad.Get();
				MeshColor = FVector(1,1,0);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),RifleLinearColor);
				if (ItemPadWeapon && RifleMesh)
					ItemPadWeapon->SetSkeletalMesh(RifleMesh);
				break;
			default:
				break;
			}
			ItemPad->SetStaticMesh(PadMesh);
			ItemPad->SetVectorParameterValueOnMaterials(TEXT("LaserColor"),MeshColor);
		}
	};
}

#endif

// Called when the game starts or when spawned
void AJMSItemPad::BeginPlay()
{
	Super::BeginPlay();

	if (ItemPadDataTable != nullptr && ItemPadNiagaraEffect != nullptr)
	{
		FItemPad* ItemPadStruct = ItemPadDataTable->FindRow<FItemPad>(ItemHandle.RowName,TEXT("GetRow"),true);
		if (ItemPadStruct)
		{
			UStaticMesh* PadMesh = nullptr;
			FVector MeshColor = FVector::ZeroVector;
			switch (ItemPadStruct->ItemType)
			{
			case EItemPadType::Health:
				PadMesh = HealthPad.Get();
				MeshColor = FVector(0,0,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),HealthLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(nullptr);
				break;
			case EItemPadType::Shield:
				PadMesh = ShieldPad.Get();
				MeshColor = FVector(0,1,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),ShieldLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(nullptr);
				break;
			case EItemPadType::PistolClip:
				PadMesh = PistolPad.Get();
				MeshColor = FVector(1,0,1);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),PistolLinearColor);
				if (ItemPadWeapon&&PistolMesh)
					ItemPadWeapon->SetSkeletalMesh(PistolMesh);
				break;
			case EItemPadType::RifleClip:
				PadMesh = RiflePad.Get();
				MeshColor = FVector(1,1,0);
				ItemPadNiagaraEffect->SetVariableLinearColor(TEXT("User.GunPad_Color"),RifleLinearColor);
				if (ItemPadWeapon && RifleMesh)
					ItemPadWeapon->SetSkeletalMesh(RifleMesh);
				break;
			default:
				break;
			}
			ItemPad->SetStaticMesh(PadMesh);
			ItemPad->SetVectorParameterValueOnMaterials(TEXT("LaserColor"),MeshColor);
		}
	};

}

// Called every frame
void AJMSItemPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemPadWeapon)
	{
		ItemPadWeapon->AddRelativeRotation(FRotator(0,1,0));
	}
	
}
void AJMSItemPad::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor == nullptr)
	{
		return;
	}
	
	if (OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		if (ItemPadDataTable != nullptr && ItemPadNiagaraEffect )
		{
			FItemPad* ItemPadStruct = ItemPadDataTable->FindRow<FItemPad>(ItemHandle.RowName,TEXT("GetRow"),true);
			float StructAmount = ItemPadStruct->Amount;
			if (ItemPadStruct)
			{

				switch (ItemPadStruct->ItemType)
				{
				case EItemPadType::Health:
					
					Cast<ICharacterInterface>(OtherActor)->IncreaseCharacterHealth(StructAmount);
					break;
				case EItemPadType::Shield:
					Cast<ICharacterInterface>(OtherActor)->IncreaseCharacterShield(StructAmount);
					break;
				case EItemPadType::PistolClip:
					Cast<ICharacterInterface>(OtherActor)->IncreaseCharacterPistolClip(StructAmount);
					break;
				case EItemPadType::RifleClip:
					Cast<ICharacterInterface>(OtherActor)->IncreaseCharacterRifleClip(StructAmount);
					break;
				default:
					break;
				}
			}
			this->Destroy();
		};
	}

}