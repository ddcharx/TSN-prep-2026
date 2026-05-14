//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
//
//
simple Txc7
{
    parameters:
        bool sendMsgOnInit = default(false); // whether the module should send out a message on initialization
        int limit = default(2);   // another parameter with a default value
        @display("i=block/routing"); // add a default icon
        volatile double delayTime @unit(s);
    gates:
        input in;
        output out;
}
//
// Make the two module look a bit different with colorization effect.
// Use cyan for `tic', and yellow for `toc'.
//
//
//
simple Tic7 extends Txc7
{
    parameters:
        @display("i=,cyan");
        sendMsgOnInit = true;   // Tic modules should send a message on init
}
//
//
//
//
//
//
	simple Toc7 extends Txc7
{
    parameters:
        @display("i=,gold");
        sendMsgOnInit = false;  // Toc modules should NOT send a message on init
}
//
//
//
//
network Tictoc7
{
    submodules:
        tic: Tic7;  // the limit parameter is still unbound here. We will get it from the ini file
        toc: Toc7;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}