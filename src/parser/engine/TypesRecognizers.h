#pragma once

namespace GKoala
{
	template<typename T>
	struct is_vector;

	template<typename T>
	struct is_vector
	{
		static const bool value = false;
		using type = T;
	};

	//Partial Specialization
	template<typename T>
	struct is_vector<std::vector<T>>
	{
		static const bool value = true;
		using type = T;
	};

	template<typename T>
	using VectorType = typename is_vector<T>::type;

	enum Type
	{
		UNKNOWN = 0,INT = 1,FLOAT = 2,STRING = 4,VECTOR = 8,OBJECT = 16
	};

	template <typename T>
	typename std::enable_if<
		std::is_same<typename std::vector<typename T::value_type>, T>::value //checking for vector old style
		&& std::is_integral<typename T::value_type>::value, void>::type
	setType (int& type)
	{
		type = Type::VECTOR | Type::INT;
	}

	template <typename T>
	typename std::enable_if<
		std::is_same<typename std::vector<typename T::value_type>, T>::value //checking for vector old style
		&& std::is_floating_point<typename T::value_type>::value, void>::type
	setType (int& type)
	{
		type = Type::VECTOR | Type::FLOAT;
	}

	template <typename T>
	typename std::enable_if<
		std::is_same<typename std::vector<typename T::value_type>, T>::value
		&& std::is_same<typename T::value_type,std::string>::value, void>::type
	setType (int& type)
	{
		type = Type::VECTOR | Type::STRING;
	}

	template <typename T>
	typename std::enable_if<
		std::is_same<typename std::vector<typename T::value_type>, T>::value
		&& !std::is_same<typename T::value_type,std::string>::value
		&& std::is_pointer<typename T::value_type>::value
		, void>::type
	setType (int& type)
	{
		type = Type::VECTOR | Type::OBJECT;
	}

	template <typename T>
	typename std::enable_if<
		std::is_integral<T>::value, void>::type
		setType (int& type)
	{
		type = Type::INT;
	}

	template <typename T>
	typename std::enable_if<
		std::is_floating_point<T>::value , void>::type
		setType (int& type)
	{
		type = Type::FLOAT;
	}

	template <typename T>
	typename std::enable_if<
	std::is_same<T,std::string>::value, void>::type
		setType (int& type)
	{
		type = Type::STRING;
	}

	template <typename T>
	typename std::enable_if<
		 !std::is_same<T,std::string>::value
		 && std::is_class<T>::value
		 && !is_vector<T>::value
		, void>::type
		setType (int& type)
	{
		type = Type::OBJECT;
	}

}


