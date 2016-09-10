configuration=
{
	daemon=false,
	pathSeparator="/",

	logAppenders=
	{
		{
			name="console appender",
			type="coloredConsole",
			level=6
		},
	},
	
	applications=
	{
		rootDirectory="applications",
		{
			description="FLV Playback Sample",
			name="flvplayback",
			protocol="dynamiclinklibrary",
			default=true,
			aliases=
			{
				"simpleLive",
				"vod",
				"live",
				"WeeklyQuest",
				"SOSample",
				"oflaDemo",
			},
			acceptors = 
			{
				{
					ip="0.0.0.0",
					port=1935,
					protocol="inboundRtmp"
				},
				--[[{
					ip="0.0.0.0",
					port=8081,
					protocol="inboundRtmps",
					sslKey="./ssl/server.key",
					sslCert="./ssl/server.crt"
				},
				{
					ip="0.0.0.0",
					port=8080,
					protocol="inboundRtmpt"
				},]]--
				{
					ip="0.0.0.0",
					port=6666,
					protocol="inboundLiveFlv",
					--waitForMetadata=true,
				},
				{
					ip="0.0.0.0",
					port=9999,
					protocol="inboundTcpTs"
				},
				--[[{
					ip="0.0.0.0",
					port=554,
					protocol="inboundRtsp"
				},]]--
			},
			externalStreams = 
			{
				--[[{
					uri="rtmp://edge01.fms.dutchview.nl/botr/bunny",
					localStreamName="test1",
					tcUrl="rtmp://edge01.fms.dutchview.nl/botr/bunny", --this one is usually required and should have the same value as the uri
				}]]--
			},
			validateHandshake=false,
			--[[authentication=
			{
				rtmp={
					type="adobe",
					encoderAgents=
					{
						"FMLE/3.0 (compatible; FMSc/1.0)",
					},
					usersFile="./configs/users.lua"
				},
				rtsp={
					usersFile="./configs/users.lua"
				}
			},]]--
			mediaStorage = {
				namedStorage1={
					--this storage contains all properties with their
					--default values. The only mandatory property is
					--mediaFolder
					description="Some storage",
					mediaFolder="public_html/media",
					metaFolder="public_html/metadata",
					enableStats=false,
					clientSideBuffer=15,
					keyframeSeek=false,
					seekGranularity=0.1,
				},
                --[[
				namedStorage2={
					mediaFolder="/Volumes/Storage/media/mp4",
					metaFolder="/tmp/metadata",
					seekGranularity=0.2,
					enableStats=true,
				},
				namedStorage3={
					mediaFolder="/Volumes/Storage/media/flv",
					metaFolder="/tmp/metadata",
				},
					{
					--this one doesn't have a name
					mediaFolder="/Volumes/Storage/media/mp3",
				}
                --]]
			},
		},
	}
}

