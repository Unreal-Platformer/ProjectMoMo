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
#include "InteractiveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"
#include "SlotData.h"
#include "../Public/GameInstance/MomoGameInstance.h"
#include "InteractiveActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProject_MomoCharacter

AProject_MomoCharacter::AProject_MomoCharacter()
{
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
}

void AProject_MomoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProject_MomoCharacter::SavePlayerData()
{
	USlotData* NewSaveGame = NewObject<USlotData>(); // 가비지 컬렉터 덕분에 Delete 안써도 됨.
	NewSaveGame->PlayerPos = GetActorLocation();

	if (false == UGameplayStatics::SaveGameToSlot(NewSaveGame, "SaveSlotName", 0))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame Error!"));
	}
}

void AProject_MomoCharacter::InitPlayerData()
{
	USlotData* MySaveGame = Cast<USlotData>(UGameplayStatics::LoadGameFromSlot("SaveSlotName", 0));
	if (nullptr == MySaveGame)
	{
		MySaveGame = GetMutableDefault<USlotData>(); // Gets the mutable default object of a class.
	}

	SetActorLocation(MySaveGame->PlayerPos);
}

void AProject_MomoCharacter::RewindInteractiveActor()
{
	testingInteractiveActor->ApplySkill(EAppliedSkill::Rewind);
}

void AProject_MomoCharacter::SlowInteractiveActor()
{
	testingInteractiveActor->ApplySkill(EAppliedSkill::Slow);
}

void AProject_MomoCharacter::QuickenInteractiveActor()
{
	testingInteractiveActor->ApplySkill(EAppliedSkill::Quicken);
}

void AProject_MomoCharacter::StopInteractiveActor()
{
	testingInteractiveActor->ApplySkill(EAppliedSkill::Stop);
}

void AProject_MomoCharacter::CancelSkill()
{
	testingInteractiveActor->CancelAppliedSkill();
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

		// Rewind
		EnhancedInputComponent->BindAction(RewindObject, ETriggerEvent::Started, this, &AProject_MomoCharacter::RewindInteractiveActor);

		// Slow
		EnhancedInputComponent->BindAction(SlowObject, ETriggerEvent::Started, this, &AProject_MomoCharacter::SlowInteractiveActor);

		// Quicken
		EnhancedInputComponent->BindAction(QuickenObject, ETriggerEvent::Started, this, &AProject_MomoCharacter::QuickenInteractiveActor);

		// Stop
		EnhancedInputComponent->BindAction(StopObject, ETriggerEvent::Started, this, &AProject_MomoCharacter::StopInteractiveActor);

		// Cancel
		EnhancedInputComponent->BindAction(CancelSkillKey, ETriggerEvent::Started, this, &AProject_MomoCharacter::CancelSkill);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
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