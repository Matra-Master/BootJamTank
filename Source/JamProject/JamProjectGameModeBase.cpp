

#include "JamProjectGameModeBase.h"
#include "CratePickup.h"
#include "Tank.h"

AJamProjectGameModeBase::AJamProjectGameModeBase()
{
    //Set default pawn class to your blueprint tank class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Tank_BP.Tank_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    //Set starting number of tanks
    remainingTanks= 3;
    //Initial Score Stetting
    playerScore = 0;

    InitialDifficulty = 10;
    Difficulty = InitialDifficulty;

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
virtual void AJamProjectGameModeBase::onNotify(TSubclassOf<UObject> Entity, FString Event)
{
    switch (Event)
    {
    case COLLECTED:
        //Someone COLLECTED something
        //Is the thing collecting stuff a tank?
        ATank* const ThingCollecting = Cast<ATank> (Entity);

        if(ThingCollecting)
        {
            //THe thing has unloaded itself already? (see if it still has a score > 0)
            float ThingScore = ThingCollecting->GetTankScore();
            if (ThingScore > 0)
            {
                //Increase Combo Difficulty
                combo = 
                //Try increasing it in a linear fashion
                
                //Try increasing it in a exponential way
                 
                float oldScore = ThingCollecting.GetTankScore();
                
                //if the thing is in a safe zone for unloading cargo

                    //Add its score to the playerScore
                    playerScore += ThingScore;
                    //Unload it  
                    ThingCollecting.UnloadCargo();
                    //Restart combo difficulty
                    Combo = 1;
            }
        }
    case DOWN:
        //DOWN is for a tank being shot at or some other thing like that
        
        //If the thing down is a tank
		ATank* const ThingDown= Cast<ATank>(Entity);
        if(ThingDown)
        {
            //Change the game state to Dead
            SetCurrentState(EGameplayState::EDead); 
            
            //Snaaaaaake
            //If you have no tanks left
                //Change the game state to GAMEOVER
                //if the player surpased the highscore
                    //Mark a new highscore
            //else 
                //Decrease remaining tanks
                //Unload the tank
                //Go back to playing state
        }

    //case 
    break;
        
    }
}

EGameplayState AJamProjectGameModeBase::GetCurrentState() const
{
    return CurrentState;
}

void AJamProjectGameModeBase::SetCurrentState(EGameplayState NewState)
{
    CurrentState = NewState;
}

uint8 AJamProjectGameModeBase::GetRemainingTanks() const
{
    return remainingTanks;
}

void AJamProjectGameModeBase::IncreaseRemainingTanks(uint8 value)
{
    remainingTanks += value;
}

void AJamProjectGameModeBase::DecreaseRemainingTanks(uint8 value)
{
    remainingTanks -= value;
}

float AJamProjectGameModeBase::GetHighScore() const
{
    return highScore;
}

void AJamProjectGameModeBase::IncreasePlayerScore(float value)
{
    playerScore += value;
}

float AJamProjectGameModeBase::GetPlayerScore()
{
    return playerScore;
}

void DifficultyChange( float combo, float time)
{
   //Resolve the value of T(c, t)
    
    
    

}