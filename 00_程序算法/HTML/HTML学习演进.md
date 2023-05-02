# 路由器后台Web页面绘制

## 语法框架

HTML+Javascript+CSS



## 演进

### Step1.Select元素

#### 程序：

```html
<select name="list" size="5">
  <option value="1">第一项</option>
  <option value="2">第二项</option>
  <option value="3">第三项</option>
  <option value="4">第四项</option>
  <option value="5">第五项</option>
  <option value="6">第六项</option>
  <option value="7">第七项</option>
  <option value="8">第八项</option>
  <option value="9">第九项</option>
  <option value="10">第十项</option>
</select>
```

#### 效果：

 ![image-20230317200035880](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230317200035880.png)

#### 语法：

HTML: 带有嵌套` <option> `元素的 `<select>` 元素。

`select`元素的`name`属性设置为`“list”`，用于在提交给服务器时标识表单元素。

`size `属性设置为`“5”`，它指定一次显示的可见选项的数量。

每个选项元素代表下拉列表中的一个可选项目。` value` 属性指定提交表单时发送到服务器的值，开始和结束标记之间的文本（例如，“第一项”）是显示在下拉列表中的可见标签。

总的来说，这段代码创建了一个下拉列表，其中包含` 10 `个项目，用汉字从 `1 `到` 10 `编号。



### Step2.弹出列表

#### 程序

```html
<select name="list">
  <option value="" selected disabled>请选择一项</option>
  <option value="1">第一项</option>
  <option value="2">第二项</option>
  <option value="3">第三项</option>
  <option value="4">第四项</option>
  <option value="5">第五项</option>
  <option value="6">第六项</option>
  <option value="7">第七项</option>
  <option value="8">第八项</option>
  <option value="9">第九项</option>
  <option value="10">第十项</option>
</select>
```

#### 效果

 ![image-20230317201749381](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230317201749381.png)

