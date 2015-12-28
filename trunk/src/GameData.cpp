/* 
 * File:   GameData.cpp
 * Author: Padula
 *
 * Created on March 19, 2013, 22:39 PM
 */
 
#include "GameData.h"

/* Pelo amor que há em Deus, não comente o bloco abaixo. Vide comentário no '.h' */
int GameData::tempoRestante = FULL_TIME;
int GameData::vidas = FULL_LIFE; 
int GameData::totalClicks = 0;
int GameData::pontos = 0;
int GameData::estado;
bool GameData::fig_invisiveis = false;
string GameData::tarefaAtual = "Memorize os objetos!";
int GameData::numTempoZerado = 0;

vector<int> GameData::posUsadasIndex;
vector<int> GameData::coordXusadas;
vector<int> GameData::coordYusadas;