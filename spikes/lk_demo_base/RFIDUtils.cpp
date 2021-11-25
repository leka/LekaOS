#include "RFIDUtils.h"

using namespace leka;

void RFIDUtils::initialize()
{
	rfidkit.init();
}

void RFIDUtils::registerEventQueue(events::EventQueue &event_queue)
{
	event_queue.call_every(300ms, this, &RFIDUtils::checkNewTag);
}

auto RFIDUtils::getTag() -> Tag
{
	return tag_id;
}

void RFIDUtils::checkNewTag()
{
	tag_container = rfidkit.getTag();
	if (std::includes(tag_container.data.begin(), tag_container.data.begin() + 4, rfid_tag_header.begin(),
					  rfid_tag_header.end())) {
		tag_container = rfidkit.getTag();
		tag_id		  = Tag(tag_container.data[7]);
		rfidkit.resetTag();

		_event_flags.set(NEW_RFID_TAG_FLAG);
	} else {
		_event_flags.clear(NEW_RFID_TAG_FLAG);
	}
}
