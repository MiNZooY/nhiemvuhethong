
## Hướng dẫn biên dịch và chạy dự án (How to build and run)

Dự án này được viết bằng C++ và sử dụng `g++` để biên dịch. Bạn cần cài đặt GCC (MinGW trên Windows hoặc g++ trên Linux/macOS) và `make`.

### 1. Biên dịch dự án (Build)

Mở Terminal (hoặc Command Prompt/PowerShell) tại thư mục chứa mã nguồn (thư mục có chứa file `Makefile` này), sau đó chạy lệnh:

```bash
make
```
Lệnh này sẽ biên dịch mã nguồn trong thư mục `src` và tạo ra file thực thi `app.exe` trong thư mục `src`.

### 2. Chạy dự án (Run)

Sau khi biên dịch xong, bạn có thể chạy ứng dụng bằng lệnh:

```bash
make run
```

Hoặc bạn có thể chạy trực tiếp file thực thi:
- **Trên Windows:** `.\src\app.exe`
- **Trên Linux/macOS:** `./src/app.exe`

### 3. Dọn dẹp (Clean)

Nếu bạn muốn xóa các file đã biên dịch để làm sạch thư mục dự án, hãy chạy lệnh:

```bash
make clean
```
