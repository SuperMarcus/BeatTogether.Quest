#pragma once

namespace BeatTogether {
    enum ConnectionFailedReason {
        // Token: 0x04000030 RID: 48
        Unknown = 1,
        // Token: 0x04000031 RID: 49
        ConnectionCanceled,
        // Token: 0x04000032 RID: 50
        ServerUnreachable,
        // Token: 0x04000033 RID: 51
        ServerAlreadyExists,
        // Token: 0x04000034 RID: 52
        ServerDoesNotExist,
        // Token: 0x04000035 RID: 53
        ServerAtCapacity,
        // Token: 0x04000036 RID: 54
        VersionMismatch,
        // Token: 0x04000037 RID: 55
        InvalidPassword,
        // Token: 0x04000038 RID: 56
        MasterServerUnreachable,
        // Token: 0x04000039 RID: 57
        MasterServerNotAuthenticated,
        // Token: 0x0400003A RID: 58
        NetworkNotConnected
    };
}
