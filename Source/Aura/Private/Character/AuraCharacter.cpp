


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	//使角色面向他们正在移动的方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//设置旋转速率
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//限制在一个特定的平面内
	GetCharacterMovement()->bConstrainToPlane = true;
	//当角色开始移动时，他们的初始位置会被“吸附”到bConstrainToPlane属性指定的平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//俯仰,翻滚,偏航不会根据控制器输入进行调整
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// PossessedBy函数在Controller获得对Pawn的控制时调用。玩家控制的角色在被玩家控制时需要初始化GAS相关的信息，以确保玩家可以立即使用技能和属性
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

// OnRep_PlayerState函数在Player State属性通过网络复制到客户端时调用
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	/*
	 * pawn 被 controller拥有时 = 这个角色被控制时,所需的技能已经准备好并可以使用
	 * AuraPlayerState作为Owner Actor，拥有逻辑状态和数据。
	 * this（即当前的AAuraCharacter实例）作为Avatar Actor，负责展示和执行技能效果。
	 */
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// 在多人游戏中，服务器和客户端可能由于各种原因出现状态不一致，这可能导致客户端尝试访问一个尚未在服务器上创建或已经销毁的对象
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 生命周期可能不同,销毁时间点问题要判断
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	
}
