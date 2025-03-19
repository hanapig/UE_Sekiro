
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Task_ReSlate.generated.h"

struct FPathFollowingResult;
/**
 * 自定义任务：让角色向目标移动并播放奔跑动画
 */
UCLASS()
class RPG_API UBT_Task_ReSlate : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Task_ReSlate();

	// 重写执行任务函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 如果任务需要持续检测（比如等待移动结束），需要开启Tick
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// 接受目标点的范围（到达此距离认为到达目标）
	UPROPERTY(EditAnywhere, Category = "移动设置")
	float AcceptableRadius = 100.f;

	// 奔跑动画素材（例如动画蒙太奇）
	UPROPERTY(EditAnywhere, Category = "动画设置")
	UAnimMontage* RunMontage;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	UBehaviorTreeComponent* MyOwnerComp = nullptr;
};
