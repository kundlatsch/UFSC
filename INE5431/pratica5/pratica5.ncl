<ncl id="myNCLDocID" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
	<head>
		<connectorBase id="connBaseId">
			<causalConnector id="onEndStart">
				<simpleCondition role="onEnd">
				</simpleCondition>
				<simpleAction max="unbounded" role="start">
				</simpleAction>
			</causalConnector>
		</connectorBase>
		<regionBase id="regionBase0">
			<region height="100.00%" id="background" left="0.00%" top="0.00%" width="100.00%" zIndex="1">
				<region height="80.00%" id="Rv" left="0.12%" top="0.00%" width="50.00%" zIndex="2">
				</region>
				<region height="80.00%" id="Ri" left="50.00%" top="0.00%" width="50.00%" zIndex="2">
				</region>
				<region height="20.00%" id="Rt" left="0.00%" top="80.00%" width="100.00%" zIndex="2">
				</region>
			</region>
		</regionBase>
		<descriptorBase id="descriptorBase1">
			<descriptor explicitDur="8s" id="Dv1" region="Rv">
			</descriptor>
			<descriptor explicitDur="10s" id="Di1" region="Ri">
			</descriptor>
			<descriptor explicitDur="8s" id="Dt1" region="Rt">
				<descriptorParam name="fit" value="fill"/>
				<descriptorParam name="fontColor" value="white"/>
			</descriptor>
			<descriptor explicitDur="10s" id="Dv2" region="Rv">
			</descriptor>
			<descriptor explicitDur="10s" id="Di2" region="Ri">
			</descriptor>
			<descriptor explicitDur="10s" id="Dt2" region="Rt">
				<descriptorParam name="fit" value="fill"/>
				<descriptorParam name="fontColor" value="white"/>
			</descriptor>
			<descriptor explicitDur="10s" id="Da1">
			</descriptor>
			<descriptor explicitDur="10s" id="Da2">
			</descriptor>
		</descriptorBase>
	</head>
	<body id="myBodyID">
		<media descriptor="Dv1" id="video1" src="media/video1.mpg" type="video/mpeg">
		</media>
		<media descriptor="Dv2" id="video2" src="media/video2.mpg" type="video/mpeg">
		</media>
		<media descriptor="Di1" id="img1" src="media/imagem1.jpg" type="image/jpeg">
		</media>
		<media descriptor="Di2" id="img2" src="media/imagem2.gif" type="image/gif">
		</media>
		<media descriptor="Dt1" id="text1" src="media/legenda1.txt" type="text/plain">
		</media>
		<media descriptor="Dt2" id="text2" src="media/legenda2.txt" type="text/plain">
		</media>
		<port component="video1" id="port1">
		</port>
		<port component="img1" id="port2">
		</port>
		<port component="text1" id="port3">
		</port>
		<media descriptor="Da1" id="audio1" src="media/audio1.mp3" type="audio/mp3">
		</media>
		<media descriptor="Da2" id="audio2" src="media/audio2.mp3" type="audio/mp3">
		</media>
		<port component="audio1" id="port4">
		</port>
		<link id="link1" xconnector="onEndStart">
			<bind component="video1" role="onEnd">
			</bind>
			<bind component="video2" role="start">
			</bind>
		</link>
		<link id="link2" xconnector="onEndStart">
			<bind component="img1" role="onEnd">
			</bind>
			<bind component="img2" role="start">
			</bind>
			<bind component="text2" role="start">
			</bind>
			<bind component="audio2" role="start">
			</bind>
		</link>
	</body>
</ncl>
