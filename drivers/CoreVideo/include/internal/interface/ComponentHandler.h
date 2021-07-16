#ifndef _LEKA_OS_DRIVER_COMPONENT_HANDLER_H
#define _LEKA_OS_DRIVER_COMPONENT_HANDLER_H

namespace leka {

template <typename T>
class ComponentHandler
{
  public:
	virtual auto getHandle() -> T &;

  protected:
	T _handle;
};

template <typename T>
auto ComponentHandler<T>::getHandle() -> T &
{
	return _handle;
}

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_COMPONENT_HANDLER_H