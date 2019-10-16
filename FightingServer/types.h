#include <qnumeric.h>

//client request code starts with 1, server response code starts with 2
const qint8 CREATE_LOBBY = 100;
const qint8 LOBBY_CREATED = 200;
const qint8 GET_LOBBY_LIST = 101;
const qint8 LOBBY_LIST_GOT = 201;
const qint8 JOIN_LOBBY = 102;
const qint8 LOBBY_JOINED = 202;
const qint8 GAME_UPDATE = 210;
const qint8 KEY_PRESS = 103;

const qint8 SERVER_ERROR = 500;