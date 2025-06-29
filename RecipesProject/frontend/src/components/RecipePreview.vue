<template>
  <div class="card h-100 recipe-preview-card" @click="viewRecipe">
    <div class="recipe-image-container position-relative">
      <img
        v-if="recipe.image"
        :src="recipe.image"
        class="card-img-top recipe-image"
        :alt="recipe.title"
        :style="{ maxHeight: imageHeight + 'px' }"
        @mouseenter="showHoverEffect = true"
        @mouseleave="showHoverEffect = false"
      />
      
      <!-- Hover Overlay -->
      <div 
        v-if="showHoverEffect" 
        class="recipe-hover-overlay d-flex align-items-center justify-content-center"
      >
        <div class="text-center text-white">
          <i class="bi bi-eye display-6"></i>
          <p class="mb-0 mt-2">Click to view recipe</p>
        </div>
      </div>
      
      <!-- Favorite Indicator -->
      <div v-if="recipe.isFavorite" class="position-absolute top-0 end-0 m-2">
        <span class="badge bg-danger">
          <i class="bi bi-heart-fill"></i>
        </span>
      </div>
      
      <!-- Viewed Indicator -->
      <div v-if="recipe.wasWatched" class="position-absolute top-0 start-0 m-2">
        <span class="badge bg-success">
          <i class="bi bi-eye-fill"></i> Viewed
        </span>
      </div>
    </div>
    
    <div class="card-body">
      <h5 class="card-title recipe-title">{{ recipe.title }}</h5>
      
      <!-- Recipe Meta Info -->
      <div class="recipe-meta mb-2">
        <span class="badge bg-primary me-1">
          <i class="bi bi-clock"></i> {{ recipe.readyInMinutes }} minutes
        </span>
        <span class="badge bg-warning">
          <i class="bi bi-star"></i> {{ recipe.popularity }} likes
        </span>
      </div>
      
      <!-- Dietary Indicators -->
      <div class="dietary-indicators mb-2">
        <span v-if="recipe.vegan" class="badge bg-success me-1">
          <i class="bi bi-leaf"></i> Vegan
        </span>
        <span v-else-if="recipe.vegetarian" class="badge bg-info me-1">
          <i class="bi bi-flower1"></i> Vegetarian
        </span>
        <span v-if="recipe.glutenFree" class="badge bg-warning">
          <i class="bi bi-shield-check"></i> Gluten-Free
        </span>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "RecipePreview",
  props: {
    recipe: {
      type: Object,
      required: true
    },
    imageHeight: {
      type: [String, Number],
      default: 120
    },
    route: {
    type: [String, Object],
    default: null
    }
  },
  data() {
    return {
      showHoverEffect: false
    }
  },
  methods: {
    viewRecipe() {
      if (this.route) {
        this.$router.push(this.route);
        return;
      }
      let path;

      if (this.recipe.recipeID && this.recipe.recipeID.startsWith('RU')) {
        const isFamily = this.recipe.isFamily === true || this.recipe.isFamily === 1;
        if (isFamily) {
          path = `/user/family-recipes/${this.recipe.recipeID}`;
        } else {
          path = `/user/my-recipes/${this.recipe.recipeID}`;
        }
      } else { // API
        path = `/recipes/fullview/${this.recipe.recipeID}`;
      }

      this.$router.push(path);
    }

  }
}
</script>

<style scoped>
.recipe-preview-card {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: none;
  border-radius: 15px;
  overflow: hidden;
}

.recipe-preview-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.15) !important;
}

.recipe-image-container {
  position: relative;
  overflow: hidden;
}

.recipe-image {
  width: 100%;
  height: auto;
  max-height: 120px;
  object-fit: cover;
  transition: transform 0.3s ease;
}

.recipe-preview-card:hover .recipe-image {
  transform: scale(1.05);
}

.recipe-hover-overlay {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.7);
  opacity: 0;
  animation: fadeIn 0.3s ease forwards;
}

@keyframes fadeIn {
  to {
    opacity: 1;
  }
}

.recipe-title {
  font-size: 1.1rem;
  font-weight: 600;
  color: #333;
  margin-bottom: 0.5rem;
  line-height: 1.3;
  display: -webkit-box;
  -webkit-line-clamp: 2;
  -webkit-box-orient: vertical;
  overflow: hidden;
}

.recipe-meta {
  display: flex;
  flex-wrap: wrap;
  gap: 0.25rem;
}

.dietary-indicators {
  display: flex;
  flex-wrap: wrap;
  gap: 0.25rem;
}

.btn {
  border-radius: 50%;
  width: 35px;
  height: 35px;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.2s ease;
}

.btn:hover {
  transform: scale(1.1);
}

.badge {
  font-size: 0.75rem;
  padding: 0.25rem 0.5rem;
}

@media (max-width: 768px) {
  .recipe-image {
    height: 150px;
  }
  
  .recipe-title {
    font-size: 1rem;
  }
}
</style>
