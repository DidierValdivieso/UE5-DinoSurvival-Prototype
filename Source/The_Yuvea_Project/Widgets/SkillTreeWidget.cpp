#include "SkillTreeWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "SkillNodeWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/MainCharacter.h"

void USkillTreeWidget::NativeConstruct()
{
    Super::NativeConstruct();

    CharacterReference = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!CharacterReference)
        return;

}
void USkillTreeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FText FormattedText = FText::Format(FText::FromString("SKILL POINT : {0}"), FText::AsNumber(CharacterReference->SkillPoints));
    SkillPointsInfo->SetText(FormattedText);
}

FReply USkillTreeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Handled();
}