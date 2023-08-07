#include "Loggers.h"

DEFINE_LOG_CATEGORY(LogNetworkingGOG);
DEFINE_LOG_CATEGORY(LogTrafficGOG);

#if ENGINE_MINOR_VERSION < 20 && ENGINE_MAJOR_VERSION < 5
DEFINE_LOG_CATEGORY(LogOnlineAchievements);
DEFINE_LOG_CATEGORY(LogOnlineFriend);
DEFINE_LOG_CATEGORY(LogOnlineIdentity);
DEFINE_LOG_CATEGORY(LogOnlineLeaderboard);
DEFINE_LOG_CATEGORY(LogOnlinePresence);
DEFINE_LOG_CATEGORY(LogOnlineSession);
DEFINE_LOG_CATEGORY(LogOnlineExternalUI);
#endif
