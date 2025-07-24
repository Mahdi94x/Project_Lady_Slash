#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Crow.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ACrow : public APawn
{
	GENERATED_BODY()

public:
	ACrow();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

};
