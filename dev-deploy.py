#!/usr/bin/env python3

EC_MISSING_DEP = 1
EC_CONN_ERROR = 2
EC_CMD_ERROR = 3
EC_FILE_ERROR = 4

import argparse
import socket
import logging
from typing import List, Tuple
import os
import glob


logging.basicConfig(
    level=logging.DEBUG,
    format="%(levelname)s: %(message)s"
)

try:
    from ssh2.session import Session
    from ssh2.sftp import LIBSSH2_FXF_CREAT, LIBSSH2_FXF_WRITE, \
        LIBSSH2_SFTP_S_IRUSR, LIBSSH2_SFTP_S_IRGRP, LIBSSH2_SFTP_S_IWUSR, \
        LIBSSH2_SFTP_S_IROTH, LIBSSH2_SFTP_S_IWGRP, LIBSSH2_SFTP_S_IXGRP, \
        LIBSSH2_SFTP_S_IXUSR, LIBSSH2_SFTP_S_IXGRP
except:
    logging.error("SSH2 library missing!")
    logging.info("Install 'ssh2-python' using pip to run this script.")
    exit(EC_MISSING_DEP)

# Default values. Can be overridden using CLI flags
robot_address = "192.168.10.1"
robot_user = "arpirobot"
robot_pass = "arpirobot"
robot_port = 22
deploy_dir = "/home/arpirobot/CoreLib-Test"
arch = ""
config = ""

# Globals used for connection
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
session = None


def parse_arguments():
    global robot_address, robot_user, robot_pass, robot_port, deploy_dir
    parser = argparse.ArgumentParser(description="Development Deploy of ArPiRobot-CoreLib and test programs")
    parser.add_argument("arch", metavar="arch", help="Architecture", type=str, choices=["armv6", "aarch64"])
    parser.add_argument("config", metavar="config", help="Build configuration", type=str, choices=["Debug", "Release", "RelWithDebInfo", "MinSizeRel"])
    parser.add_argument("-a", metavar="address", help="Specify robot address. Default: '{0}'".format(robot_address))
    parser.add_argument("-u", metavar="username", help="Specify SSH username. Default: '{0}'".format(robot_user))
    parser.add_argument("-p", metavar="password", help="Specify SSH password. Default: '{0}'".format(robot_pass))
    parser.add_argument("-t", metavar="port", help="Specify SSH port. Default: '{0}'".format(robot_port))
    parser.add_argument("-d", metavar="deploy_dir", help="Specify directory to deploy to. This directory will be"
            "deleted if it exists. Default: '{0}'".format(deploy_dir))

    args = parser.parse_args()

    if args.a is not None:
        robot_address = args.a
    if args.u is not None:
        robot_user = args.u
    if args.p is not None:
        robot_pass = args.p
    if args.d is not None:
        deploy_dir = args.d
    if args.t is not None:
        robot_port = args.t
    arch = args.arch
    config = args.config


def connect_to_host():
    global sock
    try:
        logging.info("Connecting to robot...")
        sock.settimeout(3)
        sock.connect((robot_address, robot_port))
        sock.settimeout(10)
        logging.info(f"Connected to {robot_address}:{robot_port}")
    except (socket.timeout, TimeoutError) as e:
        logging.error(f"Timed out while connecting to {robot_address}:{robot_port}!")
        exit(EC_CONN_ERROR)


def ssh_connect():
    global sock, session
    logging.info(f"Logging into robot as user {robot_user}")
    try:
        session = Session()
        session.handshake(sock)
        session.userauth_password(robot_user, robot_pass)
    except Exception as e:
        logging.error("Logging in failed!")
        logging.debug(type(e).__name__)
        exit(EC_CONN_ERROR)
    logging.info("Logged in successfully.")


def ssh_run_command(cmd: str) -> Tuple[int, str, str]:
    global channel
    try:
        channel = session.open_session()
        channel.execute(cmd)

        stdout_buffer = bytearray()
        size, data = channel.read()
        while size > 0:
            stdout_buffer.extend(data)
            size, data = channel.read()
        
        stderr_buffer = bytearray()
        size, data = channel.read_stderr()
        while size > 0:
            stderr_buffer.extend(data)
            size, data = channel.read_stderr()

        channel.close()
        exit_code = channel.get_exit_status()

        return exit_code, stdout_buffer.decode(), stderr_buffer.decode()
    except Exception as e:
        logging.error("Executing command failed!")
        logging.debug(type(e).__name__)
        exit(EC_CMD_ERROR)


def ssh_run_command_eh(cmd: str) -> str:
    exit_code, stdout, stderr = ssh_run_command(cmd)
    if exit_code != 0:
        logging.error(f"Command failed with exit code {exit_code}")
        logging.debug(stderr)
        exit(EC_CMD_ERROR)
    return stdout

def sftp_copy_file(sftp, local_path: str, remote_dest: str):
    mode = LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | \
            LIBSSH2_SFTP_S_IRGRP | LIBSSH2_SFTP_S_IWGRP | \
            LIBSSH2_SFTP_S_IROTH
    f_flags = LIBSSH2_FXF_CREAT | LIBSSH2_FXF_WRITE
    buffer_size = 1024*1024

    remote_path = f"{remote_dest}/{os.path.basename(local_path)}"

    try:
        with open(local_path, "rb", buffer_size) as local_file:
            with sftp.open(remote_path, f_flags, mode) as remote_file:
                data = local_file.read(buffer_size)
                while data:
                    remote_file.write(data)
                    data = local_file.read(buffer_size)
    except Exception as e:
        logging.error("Copying file failed!")
        logging.debug(str(e))
        exit(EC_FILE_ERROR)

def sftp_create_dir_recursive(sftp, directory: str):

    # If directory already exists, skip creating
    try:
        sftp.opendir(directory)
        return
    except Exception as e:
        pass

    mode = LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IXUSR | \
            LIBSSH2_SFTP_S_IRGRP | LIBSSH2_SFTP_S_IWGRP | LIBSSH2_SFTP_S_IXGRP | \
            LIBSSH2_SFTP_S_IROTH | LIBSSH2_SFTP_S_IXGRP

    dirs_to_create = [directory]
    while len(dirs_to_create) > 0:
        try:
            sftp.mkdir(dirs_to_create[len(dirs_to_create) - 1], mode)
            del dirs_to_create[len(dirs_to_create) - 1]
        except Exception as e:
            # Failed. Assume the parent directory does not exist
            dirs_to_create.append(os.path.dirname(dirs_to_create[len(dirs_to_create) - 1]))

            # If two consecutive entries match, then we have reached the root directory
            # As such, the error relates to permissions.
            # Exit with error
            if dirs_to_create[len(dirs_to_create) - 2] == dirs_to_create[len(dirs_to_create) - 1]:
                logging.error("Unable to create necessary directories on remote host.")
                exit(EC_FILE_ERROR)

def sftp_copy_directory(sftp, local_path: str, remote_dest: str):
    if local_path.replace("\\", "/").endswith("/"):
        local_path = local_path[:-1]
    remote_dest = f"{remote_dest}/{os.path.basename(local_path)}"
    try:
        for r, d, files in os.walk(local_path):
            for file in files:
                local_file = f"{r}/{file}".replace("\\", "/")
                sub_remote_dest = os.path.dirname(local_file.replace(local_path.replace("\\", "/"), remote_dest))
                sftp_create_dir_recursive(sftp, sub_remote_dest)
                sftp_copy_file(sftp, local_file, sub_remote_dest)

    except Exception as e:
        logging.error("Copying directory failed!")
        logging.debug(type(e).__name__)
        exit(EC_FILE_ERROR)       

def remove_prefix(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]
    return text


def sftp_send(patterns: List[str]):
    # Send files matching the given paterns with respect to the directory the script is stored in
    script_dir = os.path.dirname(os.path.realpath(__file__))

    try:
        sftp = session.sftp_init()
        for pattern in patterns:
            for file in glob.glob(f"{script_dir}/{pattern}"):
                filename = remove_prefix(file, script_dir + '/').replace('\\', '/')
                logging.info(f"\tCopying {filename}")
                if os.path.isfile(file):
                    sftp_copy_file(sftp, file.replace("\\", "/"), deploy_dir)
                else:
                    sftp_copy_directory(sftp, file.replace("\\", "/"), deploy_dir)
    except Exception as e:
        logging.error("Failed to open SFTP session!")
        logging.debug(type(e).__name__)
        exit(EC_FILE_ERROR)



def main():       
    parse_arguments()
    connect_to_host()
    ssh_connect()

    hostname = ssh_run_command_eh("hostname")
    logging.info(f"SSH session established with {hostname.strip()}.")

    logging.info("Making robot filesystem writable.")
    ssh_run_command_eh("sudo mount -o rw,remount /")

    logging.info("Removing old deploy directory contents.")
    ssh_run_command_eh(f"mkdir -p {deploy_dir}; sudo rm -rf {deploy_dir}/*")
    
    # Copy files
    logging.info("Copying files to robot.")
    sftp_send([
        "cpp_library/build/{}/{}/*.so".format(arch, config),
        "cpp_library/build/{}/{}/testrobot".format(arch, config),
        "python_bindings/arpirobot/",
        "python_bindings/testrobot-py/",
        "start-cpp.sh",
        "start-py.sh"
    ])
    
    logging.info("Fixing permissions.")
    ssh_run_command_eh(f"chmod +x {deploy_dir}/start-cpp.sh")
    ssh_run_command_eh(f"chmod +x {deploy_dir}/start-py.sh")
    ssh_run_command_eh(f"chmod +x {deploy_dir}/testrobot")

    logging.info("Fixing line endings.")
    ssh_run_command_eh(f"sed -i 's/\r$//' {deploy_dir}/start-cpp.sh")
    ssh_run_command_eh(f"sed -i 's/\r$//' {deploy_dir}/start-py.sh")

    logging.info("Making robot filesystem readonly.")
    ssh_run_command_eh("sudo mount -o ro,remount /")

    logging.info("Done. Closing session")
    exit(0)
    
    


if __name__ == "__main__":
    main()
