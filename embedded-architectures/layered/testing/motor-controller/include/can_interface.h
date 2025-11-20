#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include <cstdint>

class CanMessage
{
public:
	// Constructor for convenience
	CanMessage(uint32_t id, uint8_t length, const uint8_t* data) : _id(id), _length(length), _data{0} 
	{
		for(uint8_t i = 0; i < length && i < 8; ++i) 
		{
			_data[i] = data[i];
		}
	}
	// spaceship operator for easy comparison in tests
	auto operator<=>(const CanMessage& other) const = default;

	// Getter for ID
	uint32_t id() const { return _id; }

	// Getter for Length
	uint8_t length() const { return _length; }

	// Getter for Data
	const uint8_t* data() const { return _data; }

private:
	uint32_t _id;
	uint8_t _length;
	uint8_t _data[8];
};

class CanInterface 
{
	public:                
		virtual ~CanInterface(void) = default;

		virtual void sendMessage(const CanMessage& message) = 0;
		virtual CanMessage receiveMessage(void) = 0;
 };

#endif /* _CAN_INTERFACE_H_ */
