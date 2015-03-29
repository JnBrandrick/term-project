#include "ClientGameState.h"

#include "../../AppWindow.h"

#include "../../Engine/Event.h"
#include "../../Engine/Controller.h"

#include "../Entities/ProperEntity.h"

#include "../GameScene.h"
#include "../ClientLobbyScene.h"
#include "../Event.h"
#include "../NetworkEntityPairs.h"
#include "CommandEntity.h"
#include "PlayerEntity.h"


#include "../../Network/Client.h"
#include "../../Network/Session.h"
#include "../../Network/NetworkEntityMultiplexer.h"

using Networking::Client;

ClientGameState::ClientGameState(int id, CommandEntity *command, GameScene* gameScene, ClientLobbyScene* lobbyScene)
    : command(command)
    ,NetworkEntity(id,NET_ENT_PAIR_SERVERGAMESTATE_CLIENTGAMESTATE)
    ,_gameScene(gameScene)
    ,_lobbyScene(lobbyScene)
{
}

ClientGameState::~ClientGameState()
{
}

void ClientGameState::onUnregister(Session* session, Message msg)
{
    // Do Nothing
}

void ClientGameState::onUpdate(Message msg)
{
    switch(msg.type)
    {
        case MSG_T_PLAYER_READY_FOR_GAME:
            /* Send back player lobby selections */
            command->notifyServerLobbySelections(_lobbyScene->getSelections());
            break;

        case MSG_T_SERVERGAMESTATE_CLIENTGAMESTATE_START_GAME_SCENE:

            /* Server has informed client of what kind of player it is before going to GameScene */
            switch(command->getPlayerMode())
            {
                case VESSEL:
                    printf("VESSEL\n");
                    break;

                case DEITY:
                    printf("DEITY\n");
                    break;

                case GHOST:
                    printf("GHOST\n");
                    break;

                default:
                    printf("NO TYPE SET\n");
                    break;
            }

            AppWindow::getInstance().removeScene(1);
            _gameScene->generateMap(*((int*) msg.data));
            AppWindow::getInstance().addScene(_gameScene);
            break;
        case MSG_T_SERVERGAMESTATE_CLIENTGAMESTATE_START_LOBBY_SCENE:
            AppWindow::getInstance().removeScene(1);
            AppWindow::getInstance().addScene(_lobbyScene);
            break;
        case MSG_T_PLAYER_CONNECTED:
            _lobbyScene->updatePlayerCount(*((int*)msg.data));
            break;
        case MSG_T_PLAYER_DISCONNECTED:
            _lobbyScene->updatePlayerCount(*((int*)msg.data));
            break;
            
        case MSG_T_LOBBY_COUNTDOWN_START:
            _lobbyScene->startTimer(*((int*)msg.data));
            break;
            
        case MSG_T_LOBBY_COUNTDOWN_STOP:
            _lobbyScene->stopTimer(*((int*)msg.data));
            break;
    }
}
