
~camPos = ControlSpec(0, 125, \lin);
(
~camPosResp = OSCresponderNode(nil, 'camPos',�
	{ | time, resp, msg|
		//[msg[1], msg[2], msg[3]].postln;
		~rotZ = msg[1];
		~rotY = msg[2];
		~zoom = msg[3];
} ).add;�
)

~zoomMax = 100
~zoomMin = 5000

(
SynthDef("help-KlankOverlapTexture2",
{|out = 0, freqs = #[0,0,0,0,0,0,0,0,0,0,0,0], rings = #[0,0,0,0,0,0,0,0,0,0,0,0], atk = 5, sus = 8, rel = 5, pan = 0|
� � var e = EnvGen.kr(Env.linen(atk, sus, rel, 1, 4), doneAction:2);
� � var i = BrownNoise.ar(0.0012);
� � var z = Klank.ar(
� � � � `[freqs, nil, rings], � �// specs
� � � � i � � � � � � � � � �// input
� � );
� � Out.ar(out, Pan2.ar(z*e, pan));
}).send(s);

r = Routine{
� � var sustain = 6, transition = 4, overlap = 5;
� � var period = transition*2+sustain/overlap;
� � 0.5.wait; � � � � � �//wait for the synthdef to be sent to the server
� � inf.do{
� � � � Synth("help-KlankOverlapTexture2", [
� � � � � � \atk, transition,
� � � � � � \sus, sustain,
� � � � � � \rel, transition,
� � � � � � \pan, 1.0.rand2
� � � � ]).setn(
� � � � � � \freqs, {6000.0.linrand+80}.dup(12),
� � � � � � \rings, {0.1.rrand(3)}.dup(12)
� � � � );
� � � � period.wait;
� � }
};

)

*((~zoom-100)/5000)

r.play
r.stop


(
~nodeResp = OSCresponderNode(nil, 'node',�
	{ | time, resp, msg|
		[msg[1], msg[2], msg[3], msg[4], msg[5]].postln;
		~xPos = msg[1];
		~yPos = msg[2];
		~zPos = msg[3];
		~dist = msg[4];
		~yPos = ~yPos+450;
		~xPos = ~xPos/600;
		~zPos = ~zPos/600;
		~chaos = msg[5];
		 
		
		Synth(\brain, [\basefreq, ~yPos, \freqenv, ~dist, \pan, ~xPos, \amp, ~zPos, \freqdec, ~chaos]);
} ).add;
)�
~nodeResp.remove
~camPosResp.remove;



(
SynthDef(\brain, {|amp = 0.1, basefreq = 400, dur = 10, freqdec = 10, freqenv = 300, pan = 0|
			var source, ampenv, impulse;
	impulse = Decay.ar(Impulse.ar(0), 0.05);
	ampenv = Env([0, 1, 0], [0.001, 3], 0);
	source = Mix([
				RLPF.ar(impulse, Decay.ar(Impulse.ar(0), 0.01*freqdec, freqenv, basefreq), 0.01/dur, 0.5*amp),
				LPF.ar(impulse, basefreq, 0.1*amp)
				]);
	
	Out.ar(0, 
	Pan2.ar(source*EnvGen.ar(ampenv, doneAction:2), 0));
}
			).add;
)			

// Test
b = Synth(\brain)
b.free			
			
