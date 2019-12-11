#include <qnumeric.h>

//client request code starts with 1, server response code starts with 2
const qint8 CREATE_LOBBY = 10;
const qint8 LOBBY_CREATED = 20;
const qint8 GET_LOBBY_LIST = 11;
const qint8 LOBBY_LIST_GOT = 21;
const qint8 JOIN_LOBBY = 12;
const qint8 LOBBY_JOINED = 22;
const qint8 GAME_UPDATE = 23;
const qint8 KEY_PRESS = 13;
const qint8 PLAYER_LEFT = 24;

//error codes
const qint8 SERVER_ERROR = 99;
