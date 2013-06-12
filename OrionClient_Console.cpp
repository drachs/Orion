#include <iostream>
#include "ClientNet.h"

using namespace std;

void LongRangeScan(void);

int main(int argc, char **argv)
{
	ClientNet *TheClientNet = new ClientNet();
    while(1)
        sleep(1000);
}

/*
void LongRangeScan(void)
	{
	std::map<std::string, Anomaly> UniverseData;

	OrionClient->send_LongRangeScan();
	OrionClient->send_ShortRangeScan();

	cout << endl << "Long Range Scan Data:" << endl;
	OrionClient->recv_LongRangeScan(UniverseData);
	for (	std::map<std::string, Anomaly>::iterator i = UniverseData.begin();
			i != UniverseData.end();
			i++)
		{
		Anomaly &Result = i->second;

		cout << "Detected object: " << Result.id << endl;
		if (Result.__isset.Position)
			{
			cout << "Position: " << Result.Position.Position.x << ", " << Result.Position.Position.y << ", " << Result.Position.Position.z << endl;
			}

		if (Result.__isset.GravityWell)
			{
			cout << "Gravity Well of Mass: " << Result.GravityWell.Mass << endl;
			}

		cout << endl;
		}

    cout << endl << endl << "Short Range Scan Data: " << endl;

    OrionClient->recv_ShortRangeScan(UniverseData);
	for (	std::map<std::string, Anomaly>::iterator i = UniverseData.begin();
			i != UniverseData.end();
			i++)
		{
		Anomaly &Result = i->second;

		cout << "Detected object: " << Result.id << endl;
		if (Result.__isset.Position)
			{
			cout << "Position: " << Result.Position.Position.x << ", " << Result.Position.Position.y << ", " << Result.Position.Position.z << endl;
			}

		if (Result.__isset.GravityWell)
			{
			cout << "Gravity Well of Mass: " << Result.GravityWell.Mass << endl;
			}

		cout << endl;
		}
	}
*/
