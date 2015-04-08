
#include <string>


namespace NINJAM
{
  // TODO: have yet to locate the code that handles username validation
  //           but experiments have shown usernames are trunctuated to 16 chars
  //           and valid chars include [a-z] , [0-9] , '-' , '_'
  // TODO: have yet to locate the code that handles channelname validation
  //           but experiments have shown channelnames may be absurdly long (> 2000 chars)
  static const int MAX_USER_NAME_LEN    = 16 ;
  static const int MAX_CHANNEL_NAME_LEN = 256 ;
  static const int MAX_DEVICE_NAME_LEN  = 256 ;
  static const int MAX_FILEPATH_LEN     = 1024 ;
} ;
