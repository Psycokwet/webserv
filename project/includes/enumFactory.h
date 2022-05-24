/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enumFactory.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#             */
/*   Updated: 2022/05/03 14:09:13 by scarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_FACTORY_H
# define ENUM_FACTORY_H
# include <string.h>

# define HAS_TYPE(current, type) ((current & type) == type)

// expansion macro for enum value definition
# define ENUM_VALUE(name,assign) name assign, // ! expanded to "name assign," ?

// expansion macro for enum to string conversion
# define ENUM_CASE(name,assign) case name: return #name; 

// expansion macro for string to enum conversion
# define ENUM_STRCMP(name,assign) if (std::string(str) == std::string(#name)) return name;

// expansion macro for enum to string conversion
# define ENUM_ITERATE(name,assign) if (HAS_TYPE(mix_value, name)) (*fun)(name, raw);

/// declare the access function and define enum values
# define DECLARE_ENUM(EnumType,ENUM_DEF) \
  enum EnumType { \
    ENUM_DEF(ENUM_VALUE) \
  }; \
  const char *GetString(EnumType dummy); \
  EnumType Get##EnumType##Value(const char *string); \

/// define the access function names
# define DEFINE_ENUM(EnumType,ENUM_DEF) \
  const char *GetString(EnumType value) \
  { \
    switch(value) \
    { \
      ENUM_DEF(ENUM_CASE) \
      default: return ""; /* handle input error */ \
    } \
  } \
  EnumType Get##EnumType##Value(const char *str) \
  { \
    ENUM_DEF(ENUM_STRCMP) \
    return (EnumType)0; /* handle input error */ \
  } \
  void iterate##EnumType##Enum(int mix_value, void *raw, void(*fun)(EnumType type, void *raw)) \
  { \
  	ENUM_DEF(ENUM_ITERATE) \
  } \


# endif