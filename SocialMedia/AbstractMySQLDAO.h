#pragma once
#include <vector>

template <typename T>
class AbstractMySQLDAO
{
protected:
	virtual void updateEntitiesHelper(Runnable runnable);
	virtual std::vector<T> getEntitiesHelper(Runnable runnable);
	virtual T resultSetToEntity(ResultSet rs);
};

template<typename T>
inline void AbstractMySQLDAO<T>::updateEntitiesHelper(Runnable runnable)
{

}

template<typename T>
inline std::vector<T> AbstractMySQLDAO<T>::getEntitiesHelper(Runnable runnable)
{
	return std::vector<T>();
}

template<typename T>
inline T AbstractMySQLDAO<T>::resultSetToEntity(ResultSet rs)
{
	return T();
}
