/**
 * Created by hedgehog on 21.01.14.
 */
import java.util.StringTokenizer
import java.util.LinkedList
import java.util.Queue


data class Edge(val from : Int, val to : Int, val cap : Long, var flow : Long, var sibling : Edge? = null) {
    fun addFlow(f : Long) {
        flow += f
        sibling!!.flow -= f
        excess[from] -= f
        excess[to] += f
    }
    fun resCap() = cap - flow
}

var edges = Array(0, {LinkedList<Edge>()})
var height = IntArray(0)
var excess = LongArray(0)
var iterators = Array(0, {LinkedList<Edge>().listIterator()})
val queue : Queue<Int> = LinkedList<Int>()
var n = 0

fun pushRelabel(v : Int) {
    if (iterators[v].hasNext()) {
        val e = iterators[v].next()
        if (height[e.from] != height[e.to] + 1 || e.resCap() == 0.toLong()) return
        iterators[v].previous()
        if (e.to > 0 && e.to < n - 1 && excess[e.to] == 0.toLong()) queue.add(e.to)
        e.addFlow(Math.min(e.resCap(), excess[e.from]))
    } else {
        height[v] =
            edges[v].filter {it.resCap() > 0} .
              fold(Integer.MAX_VALUE)
              {(res : Int, x : Edge) -> Math.min(res, height[x.to])} + 1
        iterators[v] = edges[v].listIterator()
    }
}

fun main(args: Array<String>) {
    n = nextInt()
    val m = nextInt()
    edges = Array(n, {LinkedList<Edge>()})
    height = IntArray(n)
    excess = LongArray(n)
    for (i in 1 .. m) {
        val u = nextInt() - 1
        val v = nextInt() - 1
        val c = nextInt().toLong()
        val forward = Edge(u, v, c, 0)
        val backward = Edge(v, u, 0, 0, forward)
        forward.sibling = backward
        edges[u].add(forward)
        edges[v].add(backward)
    }
    height[0] = n
    for (q in edges[0]) {
        q.addFlow(q.cap)
    }
    for (i in 1 .. n - 2) {
        if (excess[i] > 0) {
            queue.add(i)
        }
    }
    iterators = Array(n, {edges[it].listIterator()})
    while (queue.isNotEmpty()) {
        val v = queue.poll()!!
        val oldHeight = height[v]
        while (excess[v] > 0 && height[v] == oldHeight) {
            pushRelabel(v)
        }
        if (excess[v] > 0) queue.add(v)
    }
    println(excess[n - 1])
}

var st = StringTokenizer("")

fun nextToken() : String {
    while (!st.hasMoreTokens()) st = StringTokenizer(readLine())
    return st.nextToken()
}

fun nextInt() = nextToken().toInt()
