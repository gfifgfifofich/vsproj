#pragma once

/*

Blocks = actions/math/functions etc.

{
	Block active - Process, set unactive, check if connected - set next to be active
}

assemble blocks into lines
check if first block in line is active. if yes - lunch the line;
if line starts with passive node - process it async, before everything else;

1) Process passive lines
2) Process action lines in which first block is active;

*/


class block
{
public:
	struct RawConnection
	{
		uint16_t BlockId = 0;// connects to block with this id
		uint16_t NodeId = 0;// connects to blocks[BlockId].Connections[NodeId]
		uint8_t DataType = 0;// 0 - action, 1 float, 2 int, 3 bool, 4 vec2, 5 vec3, 6 vec4,7 text

		bool connected = false;
		bool NeedConnection = true;
		bool source = false;
	};
	struct aConnection
	{
		RawConnection rc;
	};
	struct fConnection
	{
		RawConnection rc;
		float value = 0.0f;
	};

	std::vector<RawConnection*> Connections;

	std::vector<aConnection> ActionConnections;
	std::vector<fConnection> FloatConnections;

	bool passive = false; // if yes, needs preprocess, else - will be processed cuz action line
	bool event = false; 
	bool activated = false;

	//Draw info
	glm::vec2 position = { 0.0f,0.0f };
	std::string Name = "NoName";


	void BlockProcess()
	{
		//Get Inputs
		//Do preprocess stuff
		Process();
		//Do stuff after process
	}

	virtual void Process();

};

inline std::vector<block> blocks; // all blocks in scene

class BlockLine
{
public:
	std::vector<block*> blocks;

	bool passive = false;// is this line active, and has a flow from event (false), or is it just line for data, that can/need to be run async (true)
		
	bool start = false;// is first block is event/begining or not. 

};
inline std::vector<BlockLine> blockLines;

void BlockScheme_Compile(); // compile scene, check all block connections, create and link lines.
void BlockScheme_Process(); // Process all lines
