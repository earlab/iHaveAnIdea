
~camPos = ControlSpec(0, 125, \lin);
(
~camPosResp = OSCresponderNode(nil, 'camPos',Ê
	{ | time, resp, msg|
		//[msg[1], msg[2], msg[3]].postln;
		~rotZ = msg[1];
		~rotY = msg[2];
		~zoom = msg[3];
} ).add;Ê
)

~zoomMax = 100
~zoomMin = 5000

(
SynthDef("help-KlankOverlapTexture2",
{|out = 0, freqs = #[0,0,0,0,0,0,0,0,0,0,0,0], rings = #[0,0,0,0,0,0,0,0,0,0,0,0], atk = 5, sus = 8, rel = 5, pan = 0|
Ê Ê var e = EnvGen.kr(Env.linen(atk, sus, rel, 1, 4), doneAction:2);
Ê Ê var i = BrownNoise.ar(0.0012);
Ê Ê var z = Klank.ar(
Ê Ê Ê Ê `[freqs, nil, rings], Ê Ê// specs
Ê Ê Ê Ê i Ê Ê Ê Ê Ê Ê Ê Ê Ê Ê// input
Ê Ê );
Ê Ê Out.ar(out, Pan2.ar(z*e, pan));
}).send(s);

r = Routine{
Ê Ê var sustain = 6, transition = 4, overlap = 5;
Ê Ê var period = transition*2+sustain/overlap;
Ê Ê 0.5.wait; Ê Ê Ê Ê Ê Ê//wait for the synthdef to be sent to the server
Ê Ê inf.do{
Ê Ê Ê Ê Synth("help-KlankOverlapTexture2", [
Ê Ê Ê Ê Ê Ê \atk, transition,
Ê Ê Ê Ê Ê Ê \sus, sustain,
Ê Ê Ê Ê Ê Ê \rel, transition,
Ê Ê Ê Ê Ê Ê \pan, 1.0.rand2
Ê Ê Ê Ê ]).setn(
Ê Ê Ê Ê Ê Ê \freqs, {6000.0.linrand+80}.dup(12),
Ê Ê Ê Ê Ê Ê \rings, {0.1.rrand(3)}.dup(12)
Ê Ê Ê Ê );
Ê Ê Ê Ê period.wait;
Ê Ê }
};

)

*((~zoom-100)/5000)

r.play
r.stop


(
~nodeResp = OSCresponderNode(nil, 'node',Ê
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
)Ê
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
			
