@globe_a = global i32 5, align 4
@globe_f = global float 0x40099999A0000000, align 4 ;global_f 值为3.2
@const_ga = constant i32 9, align 4
@array_c = private constant [3 x i32] [i32 1, i32 2, i32 3], align 4
@.str = private constant [4 x i8] c"%d\0A\00", align 1 ;"%d\n"

define i32 @test_int(i32 %0, i32 %1, float %2) {
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca float, align 4
  store i32 %0, i32* %4, align 4
  store i32 %1, i32* %5, align 4
  store float %2, float* %6, align 4
  %7 = load i32, i32* %4, align 4
  %8 = load i32, i32* %5, align 4
  %9 = add nsw i32 %7, %8
  %10 = sitofp i32 %9 to float
  %11 = load float, float* %6, align 4
  %12 = fadd float %10, %11
  %13 = fptosi float %12 to i32
  ret i32 %13
}

define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca float, align 4
  %5 = alloca i32, align 4
  %6 = alloca [3 x i32], align 4
  store i32 0, i32* %1, align 4
  store i32 4, i32* %2, align 4
  store i32 0, i32* %3, align 4
  store float 2.500000e+00, float* %4, align 4
  store i32 10, i32* %5, align 4
  %7 = bitcast [3 x i32]* %6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 4 %7, i8* align 4 bitcast ([3 x i32]* @array_c to i8*), i64 12, i1 false)
  br label %8

8:                                                
  %9 = load i32, i32* %2, align 4
  %10 = icmp sgt i32 %9, 0
  br i1 %10, label %11, label %21

11:                                               
  %12 = load i32, i32* %2, align 4
  %13 = add nsw i32 %12, -1
  store i32 %13, i32* %2, align 4
  %14 = load i32, i32* %2, align 4
  %15 = icmp sgt i32 %14, 1
  br i1 %15, label %16, label %17

16:                                               
  br label %8

17:                                              
  %18 = load i32, i32* %3, align 4
  %19 = add nsw i32 %18, 1
  store i32 %19, i32* %3, align 4
  br label %20

20:                                               
  br label %8

21:                                               
  %22 = load i32, i32* %2, align 4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %22)
  %24 = load i32, i32* %3, align 4
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %24)
  %26 = load i32, i32* %3, align 4
  %27 = add nsw i32 %26, 10
  store i32 %27, i32* %2, align 4
  %28 = load i32, i32* %2, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %28)
  %30 = load i32, i32* %3, align 4
  %31 = sub nsw i32 21, %30
  store i32 %31, i32* %2, align 4
  %32 = load i32, i32* %2, align 4
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %32)
  %34 = load i32, i32* %3, align 4
  %35 = load i32, i32* @globe_a, align 4
  %36 = mul nsw i32 %34, %35
  store i32 %36, i32* %2, align 4
  %37 = load i32, i32* %2, align 4
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %37)
  %39 = load i32, i32* @globe_a, align 4
  %40 = sdiv i32 9, %39
  store i32 %40, i32* %2, align 4
  %41 = load i32, i32* %2, align 4
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %41)
  %43 = load i32, i32* @globe_a, align 4
  %44 = srem i32 9, %43
  store i32 %44, i32* %2, align 4
  %45 = load i32, i32* %2, align 4
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %45)
  %47 = load i32, i32* %2, align 4
  %48 = load i32, i32* @globe_a, align 4
  %49 = load float, float* %4, align 4
  %50 = call i32 @test_int(i32 %47, i32 %48, float %49)
  store i32 %50, i32* @globe_a, align 4
  %51 = load i32, i32* @globe_a, align 4
  %52 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %51)
  ret i32 0
}


declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg)

declare i32 @printf(i8*, ...) 
