


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**
		*
		* Cast 检查: 函数首先尝试将 OtherActor 转换为 IAbilitySystemInterface 类型的接口。IAbilitySystemInterface 是 Unreal Engine 中用于与能力系统交互的接口，它允许访问和修改角色的能力属性。
		* 获取属性集: 如果转换成功，函数尝试获取 OtherActor 的能力系统组件中的 UAuraAttributeSet。UAuraAttributeSet 是一个自定义属性集，可能是用于存储和修改游戏中的光环效果相关的属性。
		* 修改属性: 通过 const_cast 将 const UAuraAttributeSet* 转换为 UAuraAttributeSet*，以便可以修改其属性。然后，它增加 UAuraAttributeSet 中的 Health 属性值 25。
		* 销毁 Actor: 最后，调用 Destroy() 函数，这将销毁 AAuraEffectActor 实例，意味着一旦它的作用完成，它就会从游戏中移除。
	 */
	if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutableAuraAttributeSet= const_cast<UAuraAttributeSet*>(AuraAttributeSet);		
		MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);	
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}


