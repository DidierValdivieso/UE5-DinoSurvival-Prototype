#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class USkillNodeWidget;

UCLASS()
class THE_YUVEA_PROJECT_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;		
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UCanvasPanel* CanvasPanel;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* BackgroundColor;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UTextBlock* MainText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_4;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_5;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_6;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_7;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_8;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_9;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_10;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_11;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_12;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_13;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_14;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_15;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_16;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_17;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	USkillNodeWidget* W_SkillNode_18;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_4;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_5;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_6;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_7;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_8;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_9;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_10;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_11;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_12;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_13;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_14;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_15;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_16;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UImage* Line_17;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "UI")
	UTextBlock* SkillPointsInfo;

	class AMainCharacter* CharacterReference;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
