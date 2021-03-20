# **TJCS-19 AI象棋**
---
**同济大学2021年人工智能荣誉课课设-AI象棋**


## 小组成员


高琰婷

罗格峰

屈靖恩


### 作业目标：**代码能跑起来就是胜利。**

## 项目限制

方法限制：已询问过助教，不能提交卷积网络训练的模型，所以最终大家应该都是搜索剪枝模型。

时间限制：每步最多60s

内存限制：4G


## 项目预期进度：

>了解蒙特卡洛树搜索，掌握MinMax搜索、AlphaBeta剪枝算法

>阅读开源代码并尝试移植为C++版本（或python）

>完成代码的基本运行（跑起来）

>进行初步测试，参测代码基本能力

>广泛阅读开源项目，修改参赛代码参数（Depth、棋谱棋子评价函数优化）

>进行进一步测试，重复上一步骤

>编写象棋比赛协议标准OI

>实验报告

## 项目时间表

项目起止时间：*第三周-第十三周*

## 基本算法

AlphaBeta剪枝算法是对Minimax方法的优化，它们产生的结果是完全相同的，只不过运行效率不一样。
这种方法的前提假设与Minimax也是一样的：

>1）双方都按自己认为的最佳着法行棋。

>2）对给定的盘面用一个分值来评估，这个评估值永远是从一方（搜索程序）来评价的，红方有利时给一个正数，黑方有利时给一个负数。（如果红方有利时返回正数，当轮到黑方走棋时，评估值又转换到黑方的观点，如果认为黑方有利，也返回正数，这种评估方法都不适合于常规的算法描述）。

>3）从我们的搜索程序（通常把它称为Max）看来，分值大的数表示对己方有利，而对于对方Min来说，它会选择分值小的着法。
Alpha-Beta只能用递归来实现。这个思想是在搜索中传递两个值，第一个值是Alpha，即搜索到的最好值，任何比它更小的值就没用了，因为策略就是知道Alpha的值，任何小于或等于Alpha的值都不会有所提高。

第二个值是Beta，即对于对手来说最坏的值。这是对手所能承受的最坏的结果，因为我们知道在对手看来，他总是会找到一个对策不比Beta更坏的。如果搜索过程中返回Beta或比Beta更好的值，那就够好的了，走棋的一方就没有机会使用这种策略了。

在搜索着法时，每个搜索过的着法都返回跟Alpha和Beta有关的值，它们之间的关系非常重要，或许意味着搜索可以停止并返回。

如果某个着法的结果小于或等于Alpha，那么它就是很差的着法，因此可以抛弃。因为我前面说过，在这个策略中，局面对走棋的一方来说是以Alpha为评价的。

如果某个着法的结果大于或等于Beta，那么整个节点就作废了，因为对手不希望走到这个局面，而它有别的着法可以避免到达这个局面。因此如果我们找到的评价大于或等于Beta，就证明了这个结点是不会发生的，因此剩下的合理着法没有必要再搜索。

如果某个着法的结果大于Alpha但小于Beta，那么这个着法就是走棋一方可以考虑走的，除非以后有所变化。因此Alpha会不断增加以反映新的情况。有时候可能一个合理着法也不超过Alpha，这在实战中是经常发生的，此时这种局面是不予考虑的，因此为了避免这样的局面，我们必须在博弈树的上一个层局面选择另外一个着法。


## 参考资料网址（随时更新）
**key** [象棋百科全书-计算机博弈](https://www.xqbase.com/computer.htm)

