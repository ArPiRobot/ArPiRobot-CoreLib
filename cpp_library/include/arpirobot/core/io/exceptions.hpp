/*
 * Copyright 2021 Marcus Behel
 *
 * This file is part of ArPiRobot-CoreLib.
 * 
 * ArPiRobot-CoreLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ArPiRobot-CoreLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ArPiRobot-CoreLib.  If not, see <https://www.gnu.org/licenses/>. 
 */

#pragma once

#include <exception>
#include <string>

namespace arpirobot{

    class BadHandleException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

    class OpenFailedException : public std::exception{
    public: 
        const char *what() const noexcept override;
    };

    class NotImplementedByProviderException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

    class InvalidParameterException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

    class InvalidPinException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

    class WriteFailedException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

    class ReadFailedException : public std::exception{
    public:
        const char *what() const noexcept override;
    };

}