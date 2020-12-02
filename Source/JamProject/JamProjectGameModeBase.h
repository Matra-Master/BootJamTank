
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnVolume.h"
#include "JamProjectGameModeBase.generated.h"

/**
 * The Game should play like an old NES Title
 * There should be a scoreboard and some life system with continues and stuff
 * A Player must score points by taking the falling cargo and putting it at the safe zone
 *
 * Difficulty should increase by some amount over time
 * Increased difficulty can be more enemies, more cargo or new dangers.
 * EXAMPLE: Increase fire rate by some tiny amount over time
 * 	Then make a temporal increase in fire rate based on the amount of crates that the tank has collected
 * 	that restarts everytime it unloads it's cargo.
 * 	The CATCH is that the tank score increases "synergicaly" like a combo for every box after the first 
 * 	that you accumulate before unloading ;) 
 * 
 */

//enum to store the current state of gameplay
UENUM(BlueprintType)
enum class EGameplayState : uint8 {

	EPlaying,
	EDead,
	EGameOver,
	EUnknown
};

UCLASS()
class JAMPROJECT_API AJamProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AJamProjectGameModeBase();
	
	virtual void BeginPlay() override;
	
	/** Notification Implementation
	 * Try to follow the Observer pattern specification and use it to define some rules for the game
	 * Example: An event for the tank pressing the collect button on the safe area will cause a discharge of the tank's cargo and a score increase
	 */
	virtual void onNotify(TSubclassOf<class UObject> Entity, FString Event);
	
	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = "Power")
	EGameplayState GetCurrentState() const;

	/** Sets a new playing state */
	void SetCurrentState(EGameplayState NewState);
	
	/** Returns the number of tanks remaining */
	UFUNCTION(BlueprintPure, Category = "Game")
	uint8 GetRemainingTanks() const;

	/** Increase or decrease the number of remaining tanks
	 * Useful for extending gameplay to extra lives and other stuff
	*/	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void IncreaseRemainingTanks(uint8 value);
	UFUNCTION(BlueprintCallable, Category = "Game")
	void DecreaseRemainingTanks(uint8 value);

	/** Returns the High Schore value */	
	UFUNCTION(BlueprintPure, Category = "Game")
	float GetHighScore() const;
	
	/** Increase the Player Score */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void IncreasePlayerScore(float value);

	/** Get the Player Score */
	UFUNCTION(BlueprintPure, Category = "Game")
	float GetPlayerScore();
	
	/** Modify difficulty based on an equation
	 * The equation is 
	 * D = Di - Di * T(c, t)
	 * Where D is difficulty
	 * Di is initial difficulty
	 * T is a function of Combo and Time and is a value between 0 and 1
	 * Time increases as the player is alive and playing
	 * Combo increases when the player picks up a crate
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void DifficultyChange(float combo, float time)
	
protected:

	/** A value in hundreds to show the player current score and to brag about */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	float playerScore;

	/** The classic high schore shown in all arcade games of the 90s */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float highScore;
	
	/** Combo is the score multiplier you get for stacking boxes on your tank before taking them to base
	 * It also may relate to the difficulty of the level
	 * I think it should start at 1x and scale from there like a function of the tank score
	 * not in a x2 x3 x4 by box fashion but more like 1.05x 1.2x 1.5x by tank score
	 * In the future there could be a debuff that makes it < than 1x
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float Combo;

	/** Difficulty should be a function of the time and player combo
	 * Increase linearly with transcurred time
	 * Increase cuadratically by player Combo
	 * It will work on the timers assosiated to the spawning of projectiles
	 * and maybe on the enemy spawning timer too
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float Difficulty;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	float InitialDifficulty;

	/** Number of player tanks that can be destroyed before ending the game for good */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	uint8 remainingTanks;
	
private:
	/** Keeps track of the current game state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = true))
	EGameplayState CurrentState;
};
