
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

~zoomMax = 100
~zoomMin = 5000

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
			
