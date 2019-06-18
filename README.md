### Supported baseline:
- UnrealEngine 4.19, 4.20, 4.21, 4.22
- Galaxy SDK 1.133.6
- Compiler:
	- Windows: Visual Studio 15 Update 3 or later
	- Mac: Xcode 9.4 or later

### Known issues and limitations:
- UnrealEngine 4.16 is not fully tested yet
- GalaxySDK may be initialized only once per process, so each player window must be spawned in a separate process
- A player has to be logged on to GOG backend services prior to using any features from OnlineSubsystemGOG
- Dedicated servers are not supported yet

# Installing plugin:

- Copy the plugin folder to your **"GameFolder/Plugins"**
- Unpack content of the [Galaxy SDK](https://devportal.gog.com/galaxy/components/sdk "Galaxy SDK") to **"GameFolder/Plugins/OnlineSubsystemGOG/Source/ThirdParty/GalaxySDK"**
- Enable the plugin:
	* either via UE4 Editor interface:
		* **Settings** -> **Plugins** -> **Online** -> **OnlineSubsystemGOG**
		* check **"Enabled"**
		* restart UnrealEditor

	* or modifying **&#42;.uproject** file manually as follows:
```
"Plugins":
[
	{
		"Name": "OnlineSubsystem",
		"Enabled": true
	},
	{
		"Name": "OnlineSubsystemGOG",
		"Enabled": true
	}
]
```
- Update default engine configuration file (**"GameFolder/Config/DefaultEngine.ini"**) providing values without angle brackets:

```
[OnlineSubsystem]
DefaultPlatformService=GOG

[OnlineSubsystemGOG]
ClientID=<CLIENT_ID>
ClientSecret=<CLIENT_SECRET>
; The local port used to communicate with GOG Galaxy Multiplayer server and other players. Chosen automatically if not specified. Can also be overriden with -port=<port>
; Port=<LOCAL_PORT>
; The local IP address the game would bind to. Chosen automatically if not specified. Can also be overriden with -multihome=<host>
; Host=<LOCAL_HOST>
; When authorizing with Galaxy, this flag controls whether user can play offline (using local profile from the GalaxyClient), or has to be logged on to Galaxy backend services
; When offline, achievements, stats and other data are stored locally until user is online, and some functionalities are unavailable, e.g. friends, multiplayer, rich presence
bRequireBackendAuthorization=<IS_BACKEND_AUTH_REQUIRED>

[/Script/Engine.Engine]
!NetDriverDefinitions=ClearArray
NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="/Script/OnlineSubsystemGOG.NetDriverGOG",DriverClassNameFallback="/Script/OnlineSubsystemUtils.IpNetDriver")

[/Script/Engine.GameEngine]
!NetDriverDefinitions=ClearArray
NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="/Script/OnlineSubsystemGOG.NetDriverGOG",DriverClassNameFallback="/Script/OnlineSubsystemUtils.IpNetDriver")

[/Script/OnlineSubsystemGOG.NetDriverGOG]
NetConnectionClassName="/Script/OnlineSubsystemGOG.NetConnectionGOG"
```
 Please contact your GOG.com tech representative for more info on how to obtain **ClientID** and **ClientSecret**

# Logging in:
In order to use all functionality user must be signed in GOG Galaxy Client after which game must be authorized using one of the above methods:

- Using C++, IOnlineIdentity::Login() method is provided:

```
auto onlineIdentityInterface = Online::GetIdentityInterface(TEXT("GOG"));
auto onLoginCompleteDelegateHandle = onlineIdentityInterface->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateRaw(this, &OnLoginComplete));
FOnlineAccountCredentials accountCredentials;
onlineIdentityInterface->Login(0, accountCredentials);

void OnLoginComplete(int32, bool, const FUniqueNetId&, const FString&)
{
	Online::GetIdentityInterface(TEXT("GOG"))->ClearOnLoginCompleteDelegate_Handle(0, onLoginCompleteDelegateHandle);
	...
	// Proceed with online features initialization
}
```

- Using Blueprints, you can find **Login** method under the **Online** category

# Using the Achivements:
Prior to using the achivements:
* Achivements must be defined in [GOG Devportal](https://devportal.gog.com/panel/games "GOG Devportal")
* Achivements **API Key**s as defined in Devportal should be provided to engine configuration file:

```
[OnlineSubsystemGOG]

+Achievements=ACHIEVEMENT_KEY_1
+Achievements=ACHIEVEMENT_KEY_2
... e.t.c.
```
