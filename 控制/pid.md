

位置式pid
```C
int Position_PID(int Position, int Target)
{
    static float Bias, Pwm, Intergral_Bias, Last_Bias;
    Bias = Target - Position;
    Intergral_Bias += Bias;
    Pwm = Kp*Bias + Ki*Intergral + Kd*(Bias-Last_Bias);
    Last_Bias = Bias;
    return Pwm;
}
```

增量式pid
```C
int Incremental_PI(int Velocity, int Target)
{
    static float Bias, Pwm, Last_Bias;
    Bias = Target - Velocity;
    Pwm += Velocity_KP*(Bias-Last_Bias)+Velocity_KI*Bias;
    Last_Bias = Bias;
    return Pwm;
}
```