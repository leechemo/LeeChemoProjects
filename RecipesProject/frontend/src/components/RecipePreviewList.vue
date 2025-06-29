<template>
  <div class="container">
    <h3>
      {{ title }}:
      <slot></slot>
    </h3>
    <div class="row">
      <div class="col" v-for="r in recipes" :key="r.recipeID">
        <RecipePreview class="recipePreview" :recipe="r" :imageHeight="120" :route="getRoute ? getRoute(r) : null" />
      </div>
    </div>
  </div>
</template>

<script>
import RecipePreview from "./RecipePreview.vue";
export default {
  name: "RecipePreviewList",
  components: {
    RecipePreview
  },
  props: {
    title: {
      type: String,
      required: true
    },
    recipes: {
      type: Array,
      required: true
    },
    getRoute: {
      type: Function,
      required: false
    }
  },
  mounted() {
    console.log("RecipePreviewList recipes:", this.recipes);
  }
};
</script>

<style lang="scss" scoped>
.container {
  min-height: 400px;
}
</style>