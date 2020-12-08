

#include "JamProjectGameModeBase.h"
#include "CratePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Tank.h"

AJamProjectGameModeBase::AJamProjectGameModeBase()
{
    //Set default pawn class to your blueprint tank class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Tank_BP.Tank_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    //When a player dies ask if it has tanks left
    //If so restart the game with their curent score

    //If the player has no tanks left end the game and...
        //Look for the final score of the player
        //Update the higher schore of the game
        //Maybe show a scoreboard with the last 10 or so biggest scores
}

void AJamProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    SetCurrentState(EGameplayState::EPlaying); 
    
    ATank* MyCharacter = Cast<ATank> (UGameplayStatics::GetPlayerPawn(this, 0));

    if(MyCharacter)
    {
        /* Initial difficulty has a default value here but its real one should 
         * be chosen in the main menu based on a difficulty decision of the player
         * The bigger the number the easier the game
        */
        InitialDifficulty = 10;
        Difficulty = InitialDifficulty;

        //The fastest shooting frequency posible is the max difficulty
        MaxDifficulty = 0.05f;
    
        //Set starting number of tanks
        remainingTanks= 3;
        
        //Initial Score Stetting
        playerScore = 0;
    }
    
    //Widget initialization
    if(HUDWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
        if(CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }

}

/** OBSERVER PSEUDOCODE
  virtual void onNotify(const Entity& entity, Event event)
  {
    switch (event)
    {
    case EVENT_ENTITY_FELL:
      if (entity.isHero() && heroIsOnBridge_)
      {
        unlock(ACHIEVEMENT_FELL_OFF_BRIDGE);
      }
      break;

      // Handle other events, and update heroIsOnBridge_...
    }
  }
*/
void AJamProjectGameModeBase::onNotify(TSubclassOf<UObject> Entity, FString Event)
{
    if( Event == "COLLECTED" )
    {
        //Someone COLLECTED something
        //Is the thing collecting stuff a tank?
        ATank* const ThingCollecting = Cast<ATank> (Entity);

        if(ThingCollecting)
        {
            //THe thing has unloaded itself already? (see if it still has a score > 0)
            float ThingScore= ThingCollecting->GetTankScore();
            if (ThingScore > 0)
            {

		            //THE World
		            UWorld* World = GetWorld();
		            if(World)
                {
                    //Get time of current session 
                    //float time = World->GetGameTimeInSeconds();
                    float time = World->GetTimeSeconds();
                    //Get combo from the Thing
                    float someCombo = ThingCollecting->GetCombo();
                    //Increase Combo Difficulty
                    
                    
                    Combo = ThingCollecting->GetCombo();
                    //Call the difficulty function
                    DifficultyChange(Combo, time);


                    //if the thing is in a safe zone for unloading cargo
                    if(ThingCollecting->CheckIfAtBase())
                    {
                        //Add its score to the playerScore
                        playerScore += ThingScore;
                        //Unload it  
                        ThingCollecting->UnloadCargo();
                        //Restart combo difficulty
                        Combo = 0;
                        DifficultyChange(Combo, time);
                    }
                }
            }
        }
    }
    if( Event == "DOWN")
    {
        //DOWN is for a tank being shot at or some other thing like that
        
        //If the thing down is a tank
		    ATank* const ThingDown= Cast<ATank>(Entity);
        if(ThingDown)
        {
            //Change the game state to Dead
            SetCurrentState(EGameplayState::EDead);
            //-1 life
            DecreaseRemainingTanks(1); 
            
            //Unload the cank
            ThingDown->UnloadCargo();
            //Snaaaaaake
            //If you have no tanks left
            if(GetRemainingTanks() < 1)
            {
                //Change the game state to GAMEOVER
                SetCurrentState(EGameplayState::EGameOver);
                //if the player surpased the highscore
                if(playerScore > highScore)
                {
                    //Mark a new highscore
                    highScore = playerScore;
                }
            }
            //Restart the lvl entirely, make every projectile dissappear and any enemy begone.
            //Timers also reset
        }
    }

}

//DONE
EGameplayState AJamProjectGameModeBase::GetCurrentState() const
{
    return CurrentState;
}

//DONE
void AJamProjectGameModeBase::SetCurrentState(EGameplayState NewState)
{
    CurrentState = NewState;
}

//DONE
uint8 AJamProjectGameModeBase::GetRemainingTanks() const
{
    return remainingTanks;
}

//DONE
void AJamProjectGameModeBase::IncreaseRemainingTanks(uint8 value)
{
    remainingTanks += value;
}

//DONE
void AJamProjectGameModeBase::DecreaseRemainingTanks(uint8 value)
{
    remainingTanks -= value;
}
//DONE
float AJamProjectGameModeBase::GetHighScore() const
{
    return highScore;
}
//DONE
void AJamProjectGameModeBase::IncreasePlayerScore(float value)
{
    playerScore += value;
}

//DONE
float AJamProjectGameModeBase::GetPlayerScore()
{
    return playerScore;
}

//DONE
/**
 * Difficulty modifies the frequency of projectile spawning 
 * The frequency goes up with time in a linear fashion
 * When you accumulate combo it goes even higher up to a limit of 10x combo where it stays in a fixed value of MaxDifficulty
 * Difficulty resets to its linear time based value when you lose the combo
 */
void AJamProjectGameModeBase::DifficultyChange( float combo, float time)
{
    //Define Frequency from the game time, it should be a linear inverse function
    float frequency = 1 / time;

    //Check if the player made something fancy
    if( combo > 0 )
    {
        if(combo < 10)
        {
            float operand1 = 1 - frequency;
            float operand2 = log10(combo);
            frequency += operand1 / operand2;
        }
        else
        {
            frequency = MaxDifficulty;
        }
    }

    Difficulty = frequency;
    //Difficulty could be made extensible to a hard and easy mode by making a 
    //HUD choice and setting the initial difficulty there
    
}

//DONE
float AJamProjectGameModeBase::GetDifficulty()
{
    return Difficulty;
}
