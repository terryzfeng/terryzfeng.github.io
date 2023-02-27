// Happy Birthday Josh!

TriOsc osc => ADSR e => JCRev rev => dac;
global int KEY;

e.set( 10::ms, 8::ms, .5, 500::ms );
rev.mix(0.08);

// Happy Birthday in midi notes
[ [0, 0, 2, 0, 5, 4],
  [0, 0, 2, 0, 7, 5],
  [0, 0, 12, 9, 5, 4, 2],
  [10, 10, 9, 5, 7, 5] ] @=> int notes[][];

[ [3.0, 1, 4, 4, 4, 8],
  [3.0, 1, 4, 4, 4, 8],
  [3.0, 1, 4, 4, 4, 4, 4],
  [3.0, 1, 4, 4, 4, 8] ] @=> float durations[][];

100::ms => dur noteLength;

for (0 => int i; i < notes.size(); i++)
{
    for (0 => int j; j < notes[i].size(); j++)
    {
        osc.freq( Std.mtof( 60 + KEY + notes[i][j] ) );
        e.keyOn();
        50::ms => now;
        e.keyOff();
        (durations[i][j] * noteLength)-50::ms => now;
    }
}
1::second => now;