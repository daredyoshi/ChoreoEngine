#pragma once

// this just shits the bit by x places. 
// so if you have bit 1 you can offset it. 
// This way we can create a bitfield with multiple 
// events set and then just mask them out.
#define BIT(x) (1 << x)


