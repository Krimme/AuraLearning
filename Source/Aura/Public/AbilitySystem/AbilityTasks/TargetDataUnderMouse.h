

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FmouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
	使用 AbilityTasks 的基本要求
		1. 定义动态多播委托（Dynamic Multicast Delegates）:
			在你的 AbilityTask 中定义动态多播、蓝图可分配（BlueprintAssignable）的委托，这些是你的任务的输出（OUTPUTs）。
			当这些委托被触发时，调用蓝图中的执行将在调用蓝图中恢复。

		2. 定义静态工厂函数:
			你的输入（INPUTs）由静态工厂函数定义，该函数将实例化你的任务。
			这个函数的参数定义了输入到你的任务的参数。
			工厂函数应该只实例化你的任务并可能设置起始参数，它不应该触发任何回调委托。

		3. 实现 Activate() 函数:
			这个函数应该实际启动/执行你的任务逻辑。
			在这里触发回调委托是安全的。
 */

UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	/*定义静态工厂函数*/
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName = "TargetDataUnderMouse",HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FmouseTargetDataSignature ValidData;

private:

	/*实现 Activate() 函数, 委托在Activate()里面出发回调委托是安全的*/
	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivateTag) const;
};
