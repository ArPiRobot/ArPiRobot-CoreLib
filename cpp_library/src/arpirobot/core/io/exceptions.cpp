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


#include <arpirobot/core/io/exceptions.hpp>
using namespace arpirobot;

////////////////////////////////////////////////////////////////////////////////
/// BadHandleException
////////////////////////////////////////////////////////////////////////////////
const char *BadHandleException::what() const noexcept{
    return "Bad handle for IO operation.";
}

////////////////////////////////////////////////////////////////////////////////
/// OpenFailedException
////////////////////////////////////////////////////////////////////////////////
const char *OpenFailedException::what() const noexcept {
    return "Failed to open interface.";
}

////////////////////////////////////////////////////////////////////////////////
/// NotImplementedByProviderException
////////////////////////////////////////////////////////////////////////////////
const char *NotImplementedByProviderException::what() const noexcept {
    return "Attempted to use functionality not implemented by the current IoProvider.";
}

////////////////////////////////////////////////////////////////////////////////
/// InvalidParameterException
////////////////////////////////////////////////////////////////////////////////
const char *InvalidParameterException::what() const noexcept {
    return "Invalid parameter specified.";
}

////////////////////////////////////////////////////////////////////////////////
/// InvalidPinException
////////////////////////////////////////////////////////////////////////////////
const char *InvalidPinException::what() const noexcept {
    return "Attempted to use an invalid pin number";
}

////////////////////////////////////////////////////////////////////////////////
/// WriteFailedException
////////////////////////////////////////////////////////////////////////////////
const char *WriteFailedException::what() const noexcept {
    return "Write operation failed.";
}


////////////////////////////////////////////////////////////////////////////////
/// ReadFailedException
////////////////////////////////////////////////////////////////////////////////
const char *ReadFailedException::what() const noexcept {
    return "Read operation failed.";
}
