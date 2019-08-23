#include "CASBACnetStackExampleDatabase.h"

#include <time.h> // time()
#ifdef WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#endif // WIN32



bool ExampleDatabaseBitstringValue::Resize( size_t count)  {
	this->presentValue.resize(count);
	this->bitText.resize(count);
	return false; 
}
bool ExampleDatabaseBitstringValue::SetPresentValue(size_t offset, bool value) {
	if( this->presentValue.size() < offset ) {
		return false; 
	}
	this->presentValue[offset] = value ;
	return true ;  
}
bool ExampleDatabaseBitstringValue::SetBitText(size_t offset, std::string bitText) {
	if( this->bitText.size() < offset ) {
		return false; 
	}
	this->bitText[offset] = bitText ; 
	return true; 
}
void ExampleDatabaseDateValue::Set(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday ) {
	this->presentValueYear = year; 
	this->presentValueMonth = month; 
	this->presentValueDay = day; 
	this->presentValueWeekday = weekday;
}

void ExampleDatabaseTimeValue::Set( uint8_t hour, uint8_t minute, uint8_t second, uint8_t hundrethSecond) {
	this->presentValueHour = hour;
	this->presentValueMinute = minute;
	this->presentValueSecond = second;
	this->presentValueHundrethSecond = hundrethSecond;
}




ExampleDatabase::ExampleDatabase() {
	this->Setup();
}

ExampleDatabase::~ExampleDatabase() {
	this->Setup();
}

void ExampleDatabase::Setup() {

	// Set the object name properites. 
	this->analogInput.instance = 0; 
	this->analogInput.objectName = "AnalogInput_Red";
	this->analogInput.description = "Incurments once a second";
	this->analogInput.presentValue = 1.001f;
	this->analogInput.covIncurment = 2.0f; 
	this->analogOutput.instance = 1;
	this->analogOutput.objectName = "AnalogOutput_Vermilion";
	this->analogValue.instance = 2;
	this->analogValue.objectName = "AnalogValue_Orange";
	this->analogValue.presentValue = 5.43f;
	this->analogValue.maxPresValue = 1000.0f;  
	this->analogValue.minPresValue = -1000.0f;
	this->binaryInput.instance = 3;
	this->binaryInput.objectName = "BinaryInput_Amber";
	this->binaryInput.description = "Flips once a second";
	this->binaryInput.presentValue = false;
	this->binaryOutput.instance = 4;
	this->binaryOutput.objectName = "BinaryOutput_Yellow";
	this->binaryValue.instance = 5;
	this->binaryValue.objectName = "BinaryValue_Chartreuse";
	this->binaryValue.presentValue = false;
	this->device.instance = 389999; 
	this->device.objectName = "Device_Rainbow";
	this->device.UTCOffset = 0;
	this->device.currentTimeOffset = 0;
	this->device.description = "Chipkin test BACnet IP Server device";
	this->multiStateInput.instance = 13;
	this->multiStateInput.objectName = "MultiStateInput_Green";
	this->multiStateInput.presentValue = 1;
	this->multiStateOutput.instance = 14;
	this->multiStateOutput.objectName = "MultiStateOutput_Teal";
	this->multiStateValue.instance = 19;
	this->multiStateValue.objectName = "MultiStateValue_Blue";
	this->multiStateValue.presentValue = 1;
	this->trendLog.instance = 20;
	this->trendLog.objectName = "TrendLog_Indigo";
	this->bitstringValue.instance = 39;
	this->bitstringValue.objectName = "BitstringValue_White";
	this->bitstringValue.Resize(4);
	this->bitstringValue.SetBitText(0,"One");
	this->bitstringValue.SetPresentValue(0, true) ; 
	this->bitstringValue.SetBitText(1,"Two");
	this->bitstringValue.SetPresentValue(1, false) ; 
	this->bitstringValue.SetBitText(2,"Three");
	this->bitstringValue.SetPresentValue(2, false) ; 
	this->bitstringValue.SetBitText(3,"Four");
	this->bitstringValue.SetPresentValue(3, false) ; 
	this->characterStringValue.instance = 40; 
	this->characterStringValue.objectName = "CharacterstringValue_Gray";
	this->characterStringValue.presentValue = "hello world";
	this->dateValue.instance = 42 ;
	this->dateValue.objectName = "DateValue_Brown";
	this->dateValue.Set(2018-1900, 3, 15, 4 ); 
	this->integerValue.instance = 45;
	this->integerValue.objectName = "IntegerValue_Violet";
	this->integerValue.presentValue = -12345;
	this->largeAnalogValue.instance = 46 ;  
	this->largeAnalogValue.objectName = "LargeAnalogValue_Purple";
	this->largeAnalogValue.presentValue = 123456789.85;
	this->octetStringValue.instance = 47 ;
	this->octetStringValue.objectName = "OctetstringValue_Black";
	this->octetStringValue.presentValue.push_back(0x01) ; 
	this->octetStringValue.presentValue.push_back(0x02) ; 
	this->octetStringValue.presentValue.push_back(0x03) ; 
	this->octetStringValue.presentValue.push_back(0x04) ; 
	this->octetStringValue.presentValue.push_back(0x05) ; 
	this->octetStringValue.presentValue.push_back(0x06) ; 
	this->octetStringValue.presentValue.push_back(0x07) ; 
	this->positiveIntegerValue.instance = 48 ; 
	this->positiveIntegerValue.objectName = "PositiveIntegerValue_Magenta";
	this->positiveIntegerValue.presentValue = 54321;
	this->timeValue.instance = 50;
	this->timeValue.objectName = "TimeValue_Pink";
	this->timeValue.Set(15,13,55,0);
	this->networkPort.instance = 56; 
	this->networkPort.objectName = "NetworkPort_Charcoal";
	this->LoadNetworkPortProperties() ; 
}

void ExampleDatabase::LoadNetworkPortProperties() {

	// This function loads the Network port property values needed.
	// It uses system functions to get values like the IP Address and stores them
	// in the example database

	this->networkPort.BACnetIPUDPPort = 47808;


#ifdef WIN32
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	ULONG outBufLen = 15000;
	DWORD dwRetVal = 0;
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return;
	}

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return;
		}
	}

	std::string selectedAdapterName;

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			// If this is the Ethernet port, then extract the parameters
			if (pAdapter->Type == MIB_IF_TYPE_ETHERNET) {
				// Ethernet adapter
				// Extract the ethernet parameters needed for the Network Port Object
				// IP Address
				this->networkPort.IPAddressLength = sscanf_s(pAdapter->IpAddressList.IpAddress.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPAddress[0], &this->networkPort.IPAddress[1], &this->networkPort.IPAddress[2], &this->networkPort.IPAddress[3]);
				if (strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") == 0) {
					pAdapter = pAdapter->Next;
					continue; 
				}
				// Default Gateway
				this->networkPort.IPDefaultGatewayLength = sscanf_s(pAdapter->GatewayList.IpAddress.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPDefaultGateway[0], &this->networkPort.IPDefaultGateway[1], &this->networkPort.IPDefaultGateway[2], &this->networkPort.IPDefaultGateway[3]);

				// Subnet Mask
				this->networkPort.IPSubnetMaskLength = sscanf_s(pAdapter->IpAddressList.IpMask.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPSubnetMask[0], &this->networkPort.IPSubnetMask[1], &this->networkPort.IPSubnetMask[2], &this->networkPort.IPSubnetMask[3]);

				// Interface Name
				selectedAdapterName = std::string(pAdapter->AdapterName);

				// Prepare the broadcast address
				for (size_t i = 0; i < 4; i++) {
					this->networkPort.BroadcastIPAddress[i] = ((this->networkPort.IPAddress[i] & this->networkPort.IPSubnetMask[i]) == 0 ? 0xFF : this->networkPort.IPAddress[i]);
				}

				break;
			}
			pAdapter = pAdapter->Next;
		}
	}

	if (pAdapterInfo) {
		FREE(pAdapterInfo);
	}

	// Get the address info, this is to get the DNS info
	pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
	if (pAddresses == NULL) {
		printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
		return;
	}

	if (GetAdaptersAddresses(AF_INET, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAddresses);
		pAddresses = NULL;
		return;
	}

	PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	while (pCurrAddresses && this->networkPort.IPDNSServers.size() <= 0 ) {
		std::string adapterName = std::string(pCurrAddresses->AdapterName);
		if (adapterName.compare(selectedAdapterName) == 0) {

			// Extract the DNS server details
			pDnServer = pCurrAddresses->FirstDnsServerAddress;
			while (pDnServer) {
				SOCKADDR * sockaddr = pDnServer->Address.lpSockaddr;
				if (sockaddr != NULL && sockaddr->sa_family == AF_INET) {
					SOCKADDR_IN * temp = (SOCKADDR_IN*)sockaddr;
					uint8_t * dns = new uint8_t[4];
			
					sscanf_s(inet_ntoa(temp->sin_addr), "%hhd.%hhd.%hhd.%hhd", &dns[0], &dns[1], &dns[2], &dns[3]);
					this->networkPort.IPDNSServers.push_back(dns);
					this->networkPort.IPDNSServerLength = 4; 
				}
				pDnServer = pDnServer->Next;
			}

			break;
		}

		pCurrAddresses = pCurrAddresses->Next;
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

#endif // WIN32
}


void ExampleDatabase::Loop() {

	time_t currentTime = time(0); 

	static time_t updateOnceASecondTimer = 0; 
	if (updateOnceASecondTimer + 1 <= currentTime) {
		updateOnceASecondTimer = currentTime;

		this->analogInput.presentValue += 1.001f;
	}

	this->binaryInput.presentValue = (currentTime % 2 == 1);
}


