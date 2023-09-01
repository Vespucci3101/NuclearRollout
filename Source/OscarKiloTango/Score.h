#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Score.generated.h"

USTRUCT(BlueprintType)
struct FScore
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float score;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int lastLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int position;

    FScore(){}

    FScore(FString name, float score, int lastLevel, int position)
    {
        this->name = name;
        this->score = score;
        this->lastLevel = lastLevel;
        this->position = position;
    }
};