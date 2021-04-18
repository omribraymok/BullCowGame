// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    
    SetupGame();

    // PrintLine(TEXT("The number of word in the list %i."), Words.Num());
    // PrintLine(TEXT("The number of valid words in the list %i."), Isograms.Num());
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else 
        ProcessGuess(PlayerInput); // Chaking player guess
}

void UBullCowCartridge::SetupGame()
{
    // Getting the hidden word  
    HiddenWors = Isograms[FMath::RandRange(0 , Isograms.Num() - 1)];
    PrintLine(TEXT("The hiddenword is: %s."), *HiddenWors); // Debug line

    Lives = HiddenWors.Len();
    bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Guess the %i letters word!"), HiddenWors.Len() ); 
    PrintLine(TEXT("Type in your guess and\nPress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWors)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    --Lives;

    if(Lives > 0)
    {
        PrintLine(TEXT("Lost a life!, %i lives remaining"), Lives);
        if (HiddenWors.Len() != Guess.Len()) // Check if it's the same number of charcter
            PrintLine(TEXT("The hidden word is %i letters long."), HiddenWors.Len() );
        else if (!IsIsogram(Guess)) // Check if Isogram
            PrintLine(TEXT("No repeating letters in the hidden word"));
        else
        {
            FBullCowCount Score = GetBullCows(Guess);
            PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
        }
        
        PrintLine(TEXT("Try guessing again!"));
        return;
    }

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hiddenword is: %s."), *HiddenWors);
        EndGame();
        return;
    }
}

// Check if a word is Isogram, return false if it's not Isogram and true otherwise
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0 ; i < Word.Len() ; i++)
    {
        for (int32 j = i + 1 ; j < Word.Len() ; j++)
        {
            if (Word[i] == Word[j])
                return false;
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
            ValidWords.Emplace(Word);
            
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWors[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
         for (int32 HiddenWorsIndex = 0; HiddenWorsIndex < HiddenWors.Len(); HiddenWorsIndex++)
        {
            if(Guess[GuessIndex] == HiddenWors[HiddenWorsIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}