


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//Context Handle允许在应用效果时传递额外的信息(需要自行设置Add...()),比如来源,目标,触发效果的原因等
// 效果应用
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return ;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	// FGameplayEffectSpec是一个包含游戏效果所有必要数据的蓝图兼容结构，例如效果的类、持续时间、强度等.允许在不同的代码部分中传递和使用规格，而不需要担心对象的生命周期和内存管理问题
	// MakeOutgoingSpec方法用于创建一个游戏效果规格，它是从当前实体（通常是施法者）发出的，因此称为“传出”（Outgoing）
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	// ToSelf表示效果是应用到调用这个函数的对象上
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


