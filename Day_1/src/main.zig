const std = @import("std");

pub fn main() !void {
    // stdout is for the actual output of your application, for example if you
    // are implementing gzip, then only the compressed bytes should be sent to
    // stdout, not any debugging messages.
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    var file = try std.fs.cwd().openFile("input.txt", .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var buf: [1024]u8 = undefined;
    var result: i32 = 0;
    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        // Iterate in order to find the first character
        // And in opposite order to find the last character
        result += try first_number(line);
    }

    try stdout.print("Result: {d}\n", .{result});
    try bw.flush(); // don't forget to flush!
}

pub fn first_number(line: []u8) !i32 {
    var first: i32 = -1;
    var last: i32 = 0;

    for (line) |char| {
        if (char >= 0x30 and char <= 0x39) {
            var s = [1]u8{char};

            last = try std.fmt.parseUnsigned(i32, &s, 10);
            if (first == -1) {
                first = last;
            }
        }
    }
    // std.debug.print("First: {d}; Last: {d}\n", .{ first, last });
    return 10 * first + last;
}
