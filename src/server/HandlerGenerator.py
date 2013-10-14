#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re

CMD_PREFIX     = "ZZZ_"
DECLARE        = "//XXX:This is autogenerated file, do not modify!\n"
HANDLER_PREFIX = "zzz_"
BASE_INCLUDES  = ["MessageProcessor.h"]
TARGET_FILE    = "CommandHandlerAggregator.h"

scaned_handlers = []

r_match_Cmd     = re.compile("// ZZZ_TARGET_CMD:\s+(\S+)")
r_match_Handler = re.compile("THMessagePtr\s+(zzz_\S+?)\s*\(")
r_match_CmdDict = re.compile("enum\s+Command.*?\{(.*?)\}", re.DOTALL)

handler_reg_start = "bool InitCommandHandlers()\n{\n    bool result = true;\n"
handler_reg_end = "    return  result;\n}\n"
fmt_handler = "    result &= RegisterCommandHandler((Command)%d, &%s);\n"
fmt_include = '#include "%s"'
def FormatIncludeFile(fn):
    """
    Format include header.
    Arguments:
    - `fn`:
    """
    return fmt_include%fn

def FormatHandlerRegister(cmd, handler):
    """Format and return string to register a handler for cmd.

    Arguments:
    - `cmd`:
    - `handler`:
    """

    return fmt_handler%(cmd, handler)

def ProcessFile(arg, dirname, fnames):
    """Handling each file
    """
    for fn in fnames:
        filename = os.path.join(dirname, fn)
        contents = ""
        cmd      = None
        handler  = None
        try:
            contents = open(filename, 'r').read()
        except :
            print("Failed to read contents of:%s, skip ..."%filename)
        else:
            result = r_match_Cmd.search(contents)
            if result:
                cmd = result.group(1)
            result = r_match_Handler.search(contents)
            if result:
                handler = result.group(1)
            if cmd and result:
                scaned_handlers.append(dict(I=filename, C=cmd, H=handler))
        finally:
            print("\tFile: %s processed"%(filename))
    pass

gCommandDict = {}
def ParseKVPair(item):
    """
    parse kv pair, and store into gComandDict
    Arguments:
    - `item`:
    """
    pair = item.strip().split('=')
    if len(pair) != 2:
        return

    try:
        gCommandDict[pair[0].strip()] = int(pair[1].strip())
    except:
        print("Invalid item: %s\n"%(item))

def GetCommandDict():
    """Parse proto file to get command dictionary.
    """
    contents = None
    try:
        contents = open("../public/ProtocolBuffer/THMessage.proto").read()
    except :
        contents = None
    if contents is None:
        return

    result = r_match_CmdDict.search(contents)
    if result:
        commands = result.group(1)
        if commands:
            commands = commands.strip()
    if not commands:
        return

    # Now we have real contents of Command Defination, parse and store it.
    map(ParseKVPair, commands.split(";"))


if __name__ == '__main__':

    GetCommandDict() #XXX: Check return code!

    contents = []
    contents.append(DECLARE) #XXX: Add copyright

    # Basic Header .
    for header in BASE_INCLUDES:
        contents.append(FormatIncludeFile(header))

    contents.append("\n")

    print("\nScanning local handlers located in directory: CommandHandlers\n")

    os.path.walk("CommandHandlers", ProcessFile, None)

    handler_init_function = handler_reg_start;
    for target in scaned_handlers:
        include = target.get('I')
        cmdStr  = target.get('C')
        handler = target.get('H')
        if not cmdStr or not handler or not include:
            print("Invalid target: %s\n"%(str(target)))
            continue

        contents.append(FormatIncludeFile(include))
        cmd = gCommandDict.get(cmdStr)
        if cmd is None:
            print("Can find ENUM for Command: %s\n"%(cmdStr))
            continue
        handler_init_function += FormatHandlerRegister(cmd, handler)

    handler_init_function += handler_reg_end
    contents.append(handler_init_function)

    fd = open(TARGET_FILE, "w")
    if fd is not None:
        for item in contents:
            fd.write(item + "\n")
        fd.flush()
        fd.close()

    print("\nAll work done!\n")
