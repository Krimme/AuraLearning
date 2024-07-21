


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
