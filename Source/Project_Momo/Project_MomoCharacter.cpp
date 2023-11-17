// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_MomoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"
#include "SlotData.h"
#include "InteractiveActor.h"
#include "GameInstance/MomoGameInstance.h"
#include "ActorComponent/CharacterStatComponent.h"
#include "PlayerController/DefaultPlayerController.h"
#include "Widget/HUDView.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProject_MomoCharacter

AProject_MomoCharacter::AProject_MomoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));

}

void AProject_MomoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	DefaultPlayerController = Cast<ADefaultPlayerController>(Controller);
	if (DefaultPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(DefaultPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		DefaultPlayerController->GetHUDView()->BindCharacterStat(CharacterStat);
	}

	CharacterStat->SetLifePoint(100.f);
	CharacterStat->SetTimePoint(100.f);
}

void AProject_MomoCharacter::SavePlayerData()
{
	USlotData* NewSaveGame = NewObject<USlotData>(); // 가비지 컬렉터 덕분에 Delete 안써도 됨.
	NewSaveGame->PlayerPos = GetActorLocation();

	if (false == UGameplayStatics::SaveGameToSlot(NewSaveGame, "SaveSlotName", 0))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame Error!"));
	}
	
	CharacterStat->SetDamage(1.f);
	CharacterStat->UseTimePoint(1.f);
}

void AProject_MomoCharacter::InitPlayerData()
{
	USlotData* MySaveGame = Cast<USlotData>(UGameplayStatics::LoadGameFromSlot("SaveSlotName", 0));
	if (nullptr == MySaveGame)
	{
		MySaveGame = GetMutableDefault<USlotData>(); // Gets the mutable default object of a class.
	}

	SetActorLocation(MySaveGame->PlayerPos);

	CharacterStat->SetLifePoint(CharacterStat->GetCurrentLifePoint() + 1.f);
	CharacterStat->SetTimePoint(CharacterStat->GetCurrentTimePoint() + 1.f);
}

void AProject_MomoCharacter::LineTraceObject()
{
	FVector StartLoc = FollowCamera->GetComponentLocation(); // 레이저 시작 지점.
	FVector EndLoc = StartLoc + (FollowCamera->GetForwardVector() * EffectiveRange); // 레이저 끝나는 지점.


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.

	FHitResult HitResult; // 히트 결과 값 받을 변수.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (Result == true)
		InteractiveActor = Cast<AInteractiveActor>(HitResult.GetActor());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProject_MomoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProject_MomoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProject_MomoCharacter::Look);

		// Save
		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Started, this, &AProject_MomoCharacter::SavePlayerData);

		// Load
		EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Started, this, &AProject_MomoCharacter::InitPlayerData);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProject_MomoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LineTraceObject();

	if (InteractiveActor)
		UE_LOG(LogTemp, Log, TEXT("%s"), *(InteractiveActor->GetName()));
}

void AProject_MomoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProject_MomoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}