matlab读取excel
# table操作
```matlab
RawData = readtable('number.xls'); % 读取excel表格得到table格式数据
writetable(SelectData,'SelectData.xls'); % 将table存储为xls

SelectData = table; % 创建空的表格
StartCom = RawData(1,1); % 将table的内容读取为table格式
StartCom = RawData{1,1}; % 将table的内容读取为数值
```

```matlab
class(RawData) %查看数据类型

EndCom = size(RawData);
EndCom = EndCom(1);% 列长度

for j = 1:EndCom
    if ComCount<=ComNum && RawData{j,1}==ComName(ComCount)
        % 比较，小于0.44就会存储
        if RawData{j,5}<0.44 
            ComCount = ComCount + 1;
            % 要存储的数据选择
            SelectData(SelectCount,1) = RawData(j,1);
            SelectData(SelectCount,2) = RawData(j,3);
            SelectData(SelectCount,3) = RawData(j,4);
            SelectCount = SelectCount+1;
        end
    elseif ComCount>ComNum
        break
    end
    j = j+1;
end
```