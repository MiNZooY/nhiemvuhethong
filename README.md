# Todo List Application (C++)

## Giới thiệu dự án (Project Overview)
Đây là một ứng dụng quản lý công việc (Todo List) chạy trên Console, được phát triển như một phần của Đồ án môn học **Cấu trúc Dữ liệu và Giải thuật (DSA 25CTT4)**. 

Ứng dụng này cực kỳ hữu ích cho việc lên lịch và quản lý công việc cá nhân. Không giống như các ứng dụng thông thường sử dụng thư viện chuẩn của C++ (STL), toàn bộ các cấu trúc dữ liệu nền tảng trong dự án này đều được tự xây dựng hoàn toàn từ đầu (from scratch) bằng `C++ Templates`. 

## Các cấu trúc dữ liệu được ứng dụng (Applied Data Structures)
Dự án sử dụng tối đa các cấu trúc dữ liệu để giải quyết các bài toán thực tế trong ứng dụng:
- **Linked List:** Lưu trữ danh sách các công việc linh hoạt, không giới hạn số lượng.
- **Hash Table:** Quản lý và tra cứu công việc theo `ID` với tốc độ cực nhanh $O(1)$.
- **Priority Queue (Min-Heap):** Tự động gợi ý công việc khẩn cấp nhất (Next Urgent Task) dựa trên độ ưu tiên (Priority) và Ngày đến hạn (Due Date).
- **Stack:** Tính năng hoàn tác (Undo Deletion) để khôi phục các công việc vừa vô tình bị xóa.
- **Algorithms (Sorting & Searching):** Cho phép người dùng tùy chọn sắp xếp công việc theo nhiều tiêu chí (Từ A-Z, từ Z-A, hoặc theo độ ưu tiên) bằng thuật toán QuickSort.

---

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
