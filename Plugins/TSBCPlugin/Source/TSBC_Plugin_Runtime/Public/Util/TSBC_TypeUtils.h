// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once

namespace TSBC_TypeUtils
{
    template <typename T>
    static FString EnumToString() { return StaticEnum<T>()->CppType; }

    template <typename T>
    static UEnum* FindEnumSafe(UObject* Outer, const bool bExactClass = false)
    {
        const FString Name = Outer == nullptr
                             ? StaticEnum<T>()->GetPathName()
                             : StaticEnum<T>()->CppType;

        return FindObject<UEnum>(Outer, *Name, bExactClass);
    }

    template <class T>
    static UScriptStruct* FindStructSafe(UObject* Outer, const bool bExactClass = false)
    {
        const FString Name = Outer == nullptr
                             ? T::StaticStruct()->GetPathName()
                             : T::StaticStruct()->GetName();

        return static_cast<UScriptStruct*>(StaticFindObjectChecked(
            UScriptStruct::StaticClass(),
            Outer,
            *Name,
            bExactClass));
    }
}